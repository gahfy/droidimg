#include "pictures.h"
#include "../errors/errors.h"
#include <stdlib.h>
#include <stdio.h>

static void validate_pointer(picture *restrict picture_pointer);
static uint32_t get_starting_x(
    picture *restrict picture_pointer, uint32_t x, uint32_t new_width
);
static uint32_t get_starting_y(
    picture *restrict picture_pointer, uint32_t y, uint32_t new_height
);
static uint32_t get_ending_x(
    picture *restrict picture_pointer, uint32_t x, uint32_t new_width
);
static uint32_t get_ending_y(
    picture *restrict picture_pointer, uint32_t y, uint32_t new_height
);
static void validate_coordinates(uint32_t coordinate, uint32_t size);
static void add_error_invalid_coordinates(uint32_t coordinates, uint32_t size);

uint32_t get_pixel_at(
    picture *restrict picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
) {
    add_error_message_to_queue("Failed to get the pixel of the given image");
    validate_pointer(picture_pointer);
    uint32_t starting_x = get_starting_x(picture_pointer, x, new_width);
    uint32_t starting_y = get_starting_y(picture_pointer, y ,new_height);
    uint32_t ending_x = get_ending_x(picture_pointer, x, new_width);
    uint32_t ending_y = get_ending_y(picture_pointer, y, new_height);
    uint64_t alpha = 0;
    uint64_t red = 0;
    uint64_t green = 0;
    uint64_t blue = 0;
    for(uint32_t i=starting_y; i<ending_y; i++) {
        uint64_t current_alpha = 0;
        uint64_t current_red = 0;
        uint64_t current_green = 0;
        uint64_t current_blue = 0;
        for(uint32_t j=starting_x; j<ending_x; j++) {
            uint64_t index = ((uint64_t) i) * ((uint64_t) picture_pointer->width);
            index += ((uint64_t) j);
            uint32_t pixel = picture_pointer->argb_pixels[index];
            current_alpha += pixel >> 24;
            current_red += ((pixel >> 16) & 0xFF);
            current_green += ((pixel >> 8) & 0xFF);
            current_blue += (pixel & 0xFF);
        }
        current_alpha /= (ending_x - starting_x);
        current_red /= (ending_x - starting_x);
        current_green /= (ending_x - starting_x);
        current_blue /= (ending_x - starting_x);
        alpha += current_alpha;
        red += current_red;
        green += current_green;
        blue += current_blue;
    }
    alpha /= (ending_y - starting_y);
    red /= (ending_y - starting_y);
    green /= (ending_y - starting_y);
    blue /= (ending_y - starting_y);

    uint32_t result = 0;
    result = result | (((uint8_t) alpha) << 24);
    result = result | (((uint8_t) red) << 16);
    result = result | (((uint8_t) green) << 8);
    result = result | ((uint8_t) blue);

    remove_last_error();
    return result;
}

/**
 * Checks that the given picture pointer is not NULL. If it is, the program will
 * end with an error message.
 *
 * @param picture_pointer the pointer to the picture structure to check
 */
static void validate_pointer(picture *restrict picture_pointer) {
    if(picture_pointer == NULL) {
        add_error_message_to_queue(
            "The picture structure has not been initialized.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
}

/**
 * Returns the starting x coordinate in the given picture pointer of the x
 * coordinate of a copy of the given picture pointer which would be resize to
 * the given width.
 *
 * @param picture_pointer the pointer to the picture that will be copied
 * @param x               the x coordinates in the new image
 * @param new_width       the width of the new image
 *
 * @return the starting x coordinates in the given picture pointer.
 */
static uint32_t get_starting_x(
    picture *restrict picture_pointer, uint32_t x, uint32_t new_width
) {
    validate_coordinates(x, new_width);
    uint64_t start_x = ((uint64_t) x) * ((uint64_t) picture_pointer->width);
    start_x /= (uint64_t) new_width;
    // picture_pointer->width is a uint32_t
    // x is less than new_width, so x / new_width < 1
    // Then it's safe to consider that the result is a uint32_t
    return (uint32_t) start_x;
}

/**
 * Returns the starting y coordinate in the given picture pointer of the y
 * coordinate of a copy of the given picture pointer which would be resize to
 * the given height.
 *
 * @param picture_pointer the pointer to the picture that will be copied
 * @param y               the y coordinates in the new image
 * @param new_height      the height of the new image
 *
 * @return the starting y coordinates in the given picture pointer.
 */
static uint32_t get_starting_y(
    picture *restrict picture_pointer, uint32_t y, uint32_t new_height
) {
    validate_coordinates(y, new_height);
    uint64_t start_y = ((uint64_t) y) * ((uint64_t) picture_pointer->height);
    start_y /= (uint64_t) new_height;
    // picture_pointer->height is a uint32_t
    // y is less than new_height, so y / new_height < 1
    // Then it's safe to consider that the result is a uint32_t
    return (uint32_t) start_y;
}

/**
 * Returns the ending x coordinate in the given picture pointer of the x
 * coordinate of a copy of the given picture pointer which would be resize to
 * the given width.
 *
 * @param picture_pointer the pointer to the picture that will be copied
 * @param x               the x coordinates in the new image
 * @param new_width       the width of the new image
 *
 * @return the ending x coordinates in the given picture pointer.
 */
static uint32_t get_ending_x(
    picture *restrict picture_pointer, uint32_t x, uint32_t new_width
) {
    validate_coordinates(x, new_width);
    uint64_t end_x = ((uint64_t) (x+1)) * ((uint64_t) picture_pointer->width);
    end_x /= (uint64_t) new_width;
    if(end_x == get_starting_x(picture_pointer, x, new_width))
        end_x++;
    // picture_pointer->width is a uint32_t
    // x is less than new_width, so (x+1) / new_width <= 1
    // Then it's safe to consider that the result is a uint32_t
    return (uint32_t) end_x;
}

/**
 * Returns the ending y coordinate in the given picture pointer of the y
 * coordinate of a copy of the given picture pointer which would be resize to
 * the given height.
 *
 * @param picture_pointer the pointer to the picture that will be copied
 * @param y               the y coordinates in the new image
 * @param new_height      the height of the new image
 *
 * @return the ending y coordinates in the given picture pointer.
 */
static uint32_t get_ending_y(
    picture *restrict picture_pointer, uint32_t y, uint32_t new_height
) {
    validate_coordinates(y, new_height);
    uint64_t end_y = ((uint64_t) (y+1)) * ((uint64_t) picture_pointer->height);
    end_y /= (uint64_t) new_height;
    if(end_y == get_starting_y(picture_pointer, y, new_height))
        end_y++;
    // picture_pointer->height is a uint32_t
    // y is less than new_height, so (y+1) * new_height <= 1
    // Then it's safe to consider that the result is a uint32_t
    return (uint32_t) end_y;
}

/**
 * Checks that the given coordinates and size are correct. If it is not the
 * case, the program will end with an error message.
 *
 * It will basically check that the size is higher than 0, and that coordinate
 * is less than the size.
 *
 * @param coordinate the coordinate to check
 * @param size       the size to check
 */
static void validate_coordinates(uint32_t coordinate, uint32_t size) {
    if(size == 0) {
        add_error_message_to_queue(
            "The size must be higher than 0, given size is 0.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
    if(coordinate >= size) {
        add_error_invalid_coordinates(coordinate, size);
        print_errors();
        exit(EXIT_FAILURE);
    }
}

/**
 * Adds an error message to the queue for invalid coordinates. It will make a
 * message with the given coordinate and size if memory can be allocated for
 * that string, or a default error message otherwise.
 *
 * @param coordinate The coordinate to include in the error message
 * @param size       The size to include in the error message
 */
static void add_error_invalid_coordinates(uint32_t coordinate, uint32_t size) {
    char *message = malloc(sizeof(char) * (46 + 20));
    if(message != NULL) {
        sprintf(
            message,
            "The coordinate is invalid: %d on a size of %d.\n",
            coordinate,
            size
        );
        add_error_message_to_queue(message);
    } else
        add_error_message_to_queue(
            "The given coordinate is higher than the size"
        );
}