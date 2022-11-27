#ifndef PICTURES_PICTURES_H
#define PICTURES_PICTURES_H

#include <stdint.h>

/**
 * Type which defines a picture.
 * The `argb_pixels` are the pixels of the image, grouped by 4. The uint32_t
 * represents four bytes which are:
 *
 * 1. alpha channel (on 8 bits)
 * 2. red channel (on 8 bits)
 * 3. green channel (on 8 bits)
 * 4. blue channel (on 8 bits)
 *
 * This makes the size of the `argb_pixels` pointer to be width*height*4.
 */
typedef struct picture{
    uint32_t width;        /**< The width (in pixels) of the picture */
    uint32_t height;       /**< The height (in pixels) of the picture */
    uint32_t *argb_pixels; /**< The pointer to the pixels of the picture */
} picture;

/**
 * Returns a 32 bits unsigned integer which is the color of the pixel at the
 * given x and y coordinates for an image which would be the copy of the given
 * picture, but resized to the given width and height.
 *
 * The uint32_t represents four bytes which are:
 *
 * 1. alpha channel (on 8 bits)
 * 2. red channel (on 8 bits)
 * 3. green channel (on 8 bits)
 * 4. blue channel (on 8 bits)
 *
 * @param picture_pointer the pointer of the picture from which to get the pixel
 *                        at the given position
 * @param x               the x coordinate on the new picture of the pixel to
 *                        get
 * @param y               the y coordinate on the new picture of the pixel to
 *                        get
 * @param new_width       the width of the new picture
 * @param new_height      the height of the new picture
 *
 * @return the pointer to the pixel in the new image.
 */
uint32_t get_pixel_at(
    picture *restrict picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
);

#endif  /* PICTURES_PICTURES_H */
