#include <stdint.h>
#include "picture_utils.h"

uint32_t get_pixel_at(
    picture *picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
) {
    uint32_t matching_x = (x * picture_pointer->width) / new_width;
    uint32_t matching_y = (y * picture_pointer->height) / new_height;
    uint64_t matching_index = matching_y * picture_pointer->width + matching_x;
    return picture_pointer->argb_pixels[matching_index];
}
