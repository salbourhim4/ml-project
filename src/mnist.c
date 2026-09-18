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

    Matrix *images = arena_alloc(a, num_images * sizeof(Matrix));
    Matrix *labels = arena_alloc(a, num_images * sizeof(Matrix));
    if (!images || !labels) {
        fclose(img_f);
        fclose(lbl_f);
        return data;
    }

    for (uint32_t i = 0; i < num_images; i++) {
        images[i] = matrix_create(a, rows * cols, 1);
        if (!images[i].data) {
            fclose(img_f);
            fclose(lbl_f);
            return data;
        }

        unsigned char pixel_buf[rows * cols];
        if (fread(pixel_buf, 1, rows * cols, img_f) != rows * cols) {
            fclose(img_f);
            fclose(lbl_f);
            return data;
        }
        for (uint32_t p = 0; p < rows * cols; p++) {
            matrix_set(&images[i], p, 0, pixel_buf[p] / 255.0f);
        }

        labels[i] = matrix_create(a, 10, 1);
        if (!labels[i].data) {
            fclose(img_f);
            fclose(lbl_f);
            return data;
        }
        matrix_zero(&labels[i]);

        unsigned char label_byte;
        if (fread(&label_byte, 1, 1, lbl_f) != 1 || label_byte >= 10) {
            fclose(img_f);
            fclose(lbl_f);
            return data;
        }
        matrix_set(&labels[i], label_byte, 0, 1.0f);
    }

    fclose(img_f);
    fclose(lbl_f);
    return data;
}
