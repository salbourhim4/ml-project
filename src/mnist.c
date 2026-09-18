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
