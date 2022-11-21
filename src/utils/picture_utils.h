#ifndef UTILS_PICTURE_UTILS_H
#define UTILS_PICTURE_UTILS_H

typedef struct picture{
    uint32_t width;
    uint32_t height;
    uint32_t *argb_pixels;
} picture;

uint32_t get_pixel_at(
    picture *picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
);

#endif  /* UTILS_PICTURE_UTILS_H */
