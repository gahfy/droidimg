#include "pictures.h"
#include "../errors/errors.h"
#include <stdlib.h>
#include <stdio.h>

static void validate_picture_pointer(picture *restrict picture_pointer);
static uint32_t get_pixel(
    picture *restrict picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
);
static uint32_t *init_coordinates_pointer();
static uint64_t *init_pixel_sum_pointer();
static uint32_t get_pixel_with_pointers(
    picture *restrict picture_pointer,
    uint32_t *restrict coordinates_pointer,
    uint64_t *restrict sum_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
);
static void free_coordinates_and_sum(
    uint32_t *restrict coordinates,
    uint64_t *restrict sum
);
static void set_coordinates(
    uint32_t *restrict coordinates_pointer,
    picture *restrict picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
);
static void set_pixel_sum(
    picture *restrict picture_pointer,
    uint32_t *restrict coordinates_pointer,
    uint64_t *restrict sum_pointer
);
static uint32_t divide_sum_and_return_pixel(
    uint32_t *restrict coordinates_pointer,
    uint64_t *restrict sum_pointer
);
static uint32_t get_starting_x(
    picture *restrict picture_pointer, uint32_t x, uint32_t new_width
);
static uint32_t get_ending_x(
    picture *restrict picture_pointer, uint32_t x, uint32_t new_width
);
static uint32_t get_starting_y(
    picture *restrict picture_pointer, uint32_t y, uint32_t new_height
);
static uint32_t get_ending_y(
    picture *restrict picture_pointer, uint32_t y, uint32_t new_height
);
static void add_row_pixels(
    picture *restrict picture_pointer,
    uint32_t y,
    uint32_t *restrict coordinates,
    uint64_t *restrict pixel_sum_pointer
);
static void divide_sum_and_add_to_picture_sum(
    uint64_t *restrict row_sum_pointer,
    uint64_t *restrict picture_sum_pointer,
    uint32_t *restrict coordinates_pointer
);
static void validate_coordinates(uint32_t coordinate, uint32_t size);
static void add_error_invalid_coordinates(uint32_t coordinates, uint32_t size);
static void add_pixel_to_channels(
    uint32_t pixel,
    uint64_t *restrict pixel_sum_pointer
);

picture *init_picture(uint32_t width, uint32_t height) {
    uint64_t memory_size = ((uint64_t) width) * ((uint64_t) height);
    picture *result = malloc(sizeof(picture));
    if(result != NULL) {
        result->width = width;
        result->height = height;
        result->argb_pixels = malloc(sizeof(uint32_t) * memory_size);
    }
    if(result == NULL || result->argb_pixels == NULL) {
        add_error_message_to_queue("Failed to allocate memory for the picture");
        print_errors();
        exit(EXIT_FAILURE);
    }
    return result;
}

void free_picture(picture *restrict picture_pointer) {
    if(picture_pointer != NULL) {
        if(picture_pointer->argb_pixels != NULL) {
            free(picture_pointer->argb_pixels);
        }
        free(picture_pointer);
    }
}

uint32_t get_pixel_at(
    picture *restrict picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
) {
    add_error_message_to_queue("Failed to get the pixel of the given image");
    validate_picture_pointer(picture_pointer);
    uint32_t result = get_pixel(picture_pointer, x, y, new_width, new_height);
    remove_last_error();
    return result;
}

/**
 * Checks that the given picture pointer is not NULL. If it is, the program will
 * end with an error message.
 *
 * @param picture_pointer the pointer to the picture structure to check
 */
static void validate_picture_pointer(picture *restrict picture_pointer) {
    if(picture_pointer == NULL) {
        add_error_message_to_queue(
            "The picture structure has not been initialized.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
}

/**
 * Returns the pixel at the given x and y coordinates in a picture which would
 * be the copy of the given picture resized to the given new width and height.
 *
 * @param sum             the pointer which role is to hold the sum of the
 *                        channels of the pixels of the rows of the original
 *                        picture matching the given x and y coordinates.
 * @param x               the x coordinate in the new picture of the pixel to
 *                        calculate
 * @param y               the y coordinate in the new picture of the pixel to
 *                        calculate
 * @param new_width       the width of the picture for which to calculate the
 *                        pixel
 * @param new_height      the height of the picture for which to calculate the
 *                        pixel
 *
 * @return the value of the pixel at the given x and y coordinates
 */
static uint32_t get_pixel(
    picture *restrict picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
) {
    uint32_t *coordinates = init_coordinates_pointer();
    uint64_t *sum = init_pixel_sum_pointer();
    uint32_t result = get_pixel_with_pointers(
        picture_pointer, coordinates, sum, x, y, new_width, new_height
    );
    free_coordinates_and_sum(coordinates, sum);
    return result;
}

/**
 * Initializes a pointer which will contain the coordinates of the starting and
 * ending x and y and returns it. The program will end with an error message if
 * allocation failed.
 *
 * @return the pointer which has been initialized.
 */
static uint32_t *init_coordinates_pointer() {
    uint32_t *coordinates_pointer = malloc(sizeof(uint32_t) * 4);
    if(coordinates_pointer == NULL) {
        add_error_message_to_queue(
            "Failed to allocate sufficient memory for the sum of pixels.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
    coordinates_pointer[0] = 0;
    coordinates_pointer[1] = 0;
    coordinates_pointer[2] = 0;
    coordinates_pointer[3] = 0;
    return coordinates_pointer;
}

/**
 * Initializes a pointer which will contain the sums of the channels of a pixel
 * and returns it. The program will end with an error message if allocation
 * failed.
 *
 * @return the pointer which has been initialized.
 */
static uint64_t *init_pixel_sum_pointer() {
    uint64_t *pixel_sum_pointer = malloc(sizeof(uint64_t) * 4);
    if(pixel_sum_pointer == NULL) {
        add_error_message_to_queue(
            "Failed to allocate sufficient memory for the sum of pixels.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
    pixel_sum_pointer[0] = 0;
    pixel_sum_pointer[1] = 0;
    pixel_sum_pointer[2] = 0;
    pixel_sum_pointer[3] = 0;
    return pixel_sum_pointer;
}

/**
 * Returns the pixel at the given x and y coordinates in a picture which would
 * be the copy of the given picture resized to the given new width and height.
 *
 * This method assumes that the picture and coordinates pointers are already
 * allocated in memory.
 *
 * @param picture_pointer the pointer to the picture from which to get the pixel
 * @param coordinates     the pointer which role is to hold values of the
 *                        starting and ending x and y coordinates of the
 *                        original picture matching the given x and y
 *                        coordinates in the resized copy of the picture.
 * @param sum             the pointer which role is to hold the sum of the
 *                        channels of the pixels of the rows of the original
 *                        picture matching the given x and y coordinates.
 * @param x               the x coordinate in the new picture of the pixel to
 *                        calculate
 * @param y               the y coordinate in the new picture of the pixel to
 *                        calculate
 * @param new_width       the width of the picture for which to calculate the
 *                        pixel
 * @param new_height      the height of the picture for which to calculate the
 *                        pixel
 *
 * @return the value of the pixel at the given x and y coordinates
 */
static uint32_t get_pixel_with_pointers(
    picture *restrict picture_pointer,
    uint32_t *restrict coordinates,
    uint64_t *restrict sum,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
) {
    set_coordinates(coordinates, picture_pointer, x, y, new_width, new_height);
    set_pixel_sum(picture_pointer, coordinates, sum);
    return divide_sum_and_return_pixel(coordinates, sum);
}

/**
 * Desallocates the memory allocated to the given coordinates and sum pointers.
 *
 * @param coordinates the coordinates pointer to free
 * @param sum         the sum pointer to free
 */
static void free_coordinates_and_sum(
    uint32_t *restrict coordinates,
    uint64_t *restrict sum
) {
    if(coordinates != NULL)
        free(coordinates);
    if(sum != NULL)
        free(sum);
}

/**
 * Populates the values of the given coordinates pointer. It will set the
 * starting and ending x and y to the pointer.
 *
 * @param coordinates     the pointer to populate with the coordinates
 * @param picture_pointer the pointer to the picture used to calculate the
 *                        coordinates
 * @param x               the x coordinate in the new picture used to calculate
 *                        the starting and ending x
 * @param y               the y coordinate in the new picture used to calculate
 *                        the starting and ending y
 * @param new_width       the width of the new image for which to calculate the
 *                        coordinates
 * @param new_height      the height of the new image for which to calculate the
 *                        coordinates
 */
static void set_coordinates(
    uint32_t *restrict coordinates,
    picture *restrict picture_pointer,
    uint32_t x,
    uint32_t y,
    uint32_t new_width,
    uint32_t new_height
) {
    coordinates[0] = get_starting_x(picture_pointer, x, new_width);
    coordinates[1] = get_ending_x(picture_pointer, x, new_width);
    coordinates[2] = get_starting_y(picture_pointer, y ,new_height);
    coordinates[3] = get_ending_y(picture_pointer, y, new_height);
}

/**
 * Sets the sum of the average channels of the pixels of the rows (each row
 * starting at the given starting and ending x) between given starting and
 * ending y.
 *
 * @param picture_pointer the picture from which to get the pixels which will be
 *                        used for calculation of the sum.
 * @param coordinates     the pointer containing the starting and ending x and y
 *                        coordinates
 * @param sum             the pointer which will contain the sums of the
 *                        channels of the rows
 */
static void set_pixel_sum(
    picture *restrict picture_pointer,
    uint32_t *restrict coordinates,
    uint64_t *restrict sum
) {
    for(uint32_t i=coordinates[2]; i<coordinates[3]; i++) {
        uint64_t *current_sum = init_pixel_sum_pointer();
        add_row_pixels(picture_pointer, i, coordinates, current_sum);
        divide_sum_and_add_to_picture_sum(current_sum, sum, coordinates);
        free(current_sum);
    }
}

/**
 * Divides the sum of the average channels of all rows by the number of rows,
 * and then returns the result as a 32 bits integer which represents the
 * 4 channels encoded on 8 bits.
 *
 * @param coordinates the pointer containing the starting and ending x and y
 *                    coordinates
 * @param sum         the pointer to the sum of the average channels of all rows
 *
 * @return the result of the division, which is the average pixel of the square
 */
static uint32_t divide_sum_and_return_pixel(
    uint32_t *restrict coordinates,
    uint64_t *restrict sum
) {
    sum[0] /= (coordinates[3] - coordinates[2]);
    sum[1] /= (coordinates[3] - coordinates[2]);
    sum[2] /= (coordinates[3] - coordinates[2]);
    sum[3] /= (coordinates[3] - coordinates[2]);
    uint32_t result = ((uint8_t) sum[0]) << 24;
    result = result | (((uint8_t) sum[1]) << 16);
    result = result | (((uint8_t) sum[2]) << 8);
    result = result | ((uint8_t) sum[3]);
    return result;
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
    picture *restrict picture_pointer,
    uint32_t x,
    uint32_t new_width
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
 * Adds the values of the channels of the pixels of the row to the given pointer
 * of the channels.
 *
 * @param picture_pointer     the pointer to the picture which contains the
 *                            pixels to add
 * @param y                   the y coordinate of the row containing the pixels
 *                            to add
 * @param coordinates_pointer the pointer to the coordinates of the coordinates
 *                            of the picture
 * @param pixel_sum_pointer   the pointer which contains the sum of the pixels.
 *                            It has a size of 4. Index 0 for the alpha channel,
 *                            index 1 for the red channel, index 2 for the green
 *                            channel and index 3 for the blue channel.
 */
static void add_row_pixels(
    picture *restrict picture_pointer,
    uint32_t y,
    uint32_t *restrict coordinates,
    uint64_t *restrict pixel_sum_pointer
) {
    for(uint32_t x=coordinates[0]; x<coordinates[1]; x++) {
        uint64_t index = ((uint64_t) y) * ((uint64_t) picture_pointer->width);
        index += ((uint64_t) x);
        uint32_t pixel = picture_pointer->argb_pixels[index];
        add_pixel_to_channels(pixel, pixel_sum_pointer);
    }
}

/**
 * Adds the average of the sum of the channels of the pixels of the row to the
 * picture pixels.
 *
 * @param row_sum_pointer     the pointer to the sum of the channels of the
 *                            pixels of the row
 * @param picture_sum_pointer the pointer to the sum of the channels of the
 *                            pixels of the picture
 * @param coordinates_pointer the pointer containing the coordinates of the
 *                            starting x (index 0), ending x (index 1), starting
 *                            y (index 2) and ending y (index 3).
 */
static void divide_sum_and_add_to_picture_sum(
    uint64_t *restrict row_sum_pointer,
    uint64_t *restrict picture_sum_pointer,
    uint32_t *restrict coordinates_pointer
) {
    row_sum_pointer[0] /= (coordinates_pointer[1] - coordinates_pointer[0]);
    row_sum_pointer[1] /= (coordinates_pointer[1] - coordinates_pointer[0]);
    row_sum_pointer[2] /= (coordinates_pointer[1] - coordinates_pointer[0]);
    row_sum_pointer[3] /= (coordinates_pointer[1] - coordinates_pointer[0]);
    picture_sum_pointer[0] += ((uint8_t) row_sum_pointer[0]);
    picture_sum_pointer[1] += ((uint8_t) row_sum_pointer[1]);
    picture_sum_pointer[2] += ((uint8_t) row_sum_pointer[2]);
    picture_sum_pointer[3] += ((uint8_t) row_sum_pointer[3]);
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

/**
 * Adds the value of the channels of the pixels to the given pointer of the
 * channels.
 *
 * @param pixel             the pixel to add to the channels.
 * @param pixel_sum_pointer the pointer which contains the sum of the pixels. It
 *                          has a size of 4. Index 0 for the alpha channel,
 *                          index 1 for the red channel, index 2 for the green
 *                          channel and index 3 for the blue channel.
 */
static void add_pixel_to_channels(
    uint32_t pixel,
    uint64_t *restrict pixel_sum_pointer
) {
    pixel_sum_pointer[0] += pixel >> 24;
    pixel_sum_pointer[1] += ((pixel >> 16) & 0xFF);
    pixel_sum_pointer[2] += ((pixel >> 8) & 0xFF);
    pixel_sum_pointer[3] += (pixel & 0xFF);
}