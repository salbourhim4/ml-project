#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "mnist.h"

uint32_t swap_endian (uint32_t val) {
    return ((val & 0xFF000000) >> 24) |
           ((val & 0x00FF0000) >> 8)  |
           ((val & 0x0000FF00) << 8)  |
           ((val & 0x000000FF) << 24);
}

// Reads one big-endian 32-bit field from an IDX file header and converts it
// to host byte order. Returns 1 on success, 0 on short read.
static int read_uint32_be(FILE *f, uint32_t *out) {
    uint32_t val;
    if (fread(&val, sizeof(val), 1, f) != 1) {
        return 0;
    }
    *out = swap_endian(val);
    return 1;
}

MNISTData mnist_load(Arena *a, const char *image_path, const char *label_path) {
    MNISTData data = {0};

    FILE *img_f = fopen(image_path, "rb");
    if (!img_f) {
        return data;
    }

    FILE *lbl_f = fopen(label_path, "rb");
    if (!lbl_f) {
        fclose(img_f);
        return data;
    }

    uint32_t img_magic, num_images, rows, cols;
    if (!read_uint32_be(img_f, &img_magic) || img_magic != 2051 ||
        !read_uint32_be(img_f, &num_images) ||
        !read_uint32_be(img_f, &rows) ||
        !read_uint32_be(img_f, &cols)) {
        fclose(img_f);
        fclose(lbl_f);
        return data;
    }

    uint32_t lbl_magic, num_labels;
    if (!read_uint32_be(lbl_f, &lbl_magic) || lbl_magic != 2049 ||
        !read_uint32_be(lbl_f, &num_labels)) {
        fclose(img_f);
        fclose(lbl_f);
        return data;
    }

    if (num_labels != num_images) {
        fclose(img_f);
        fclose(lbl_f);
        return data;
    }

    // TODO: allocate images/labels arrays and read per-sample pixel/label
    // data (next pass).

    fclose(img_f);
    fclose(lbl_f);
    return data;
}
