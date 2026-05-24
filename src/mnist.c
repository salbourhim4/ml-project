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
