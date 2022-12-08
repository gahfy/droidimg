#include "reader.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../files/reader.h"
#include "../files/commons.h"
#include "../errors/errors.h"
#include "../pictures/pictures.h"
#include <webp/decode.h>

static picture *read_webp_image_from_data(
    file_data *restrict file_data_pointer
);
static void validate_file_header(uint8_t *restrict file_header);
static bool is_header_of_webp(uint8_t *restrict header);
static picture *init_picture_with_webp(file_data *restrict file_data_pointer);
static void populate_picture(
    picture *restrict picture_pointer,
    file_data *restrict file_data_pointer
);
static void set_width_and_height(
    file_data *restrict file_data_pointer,
    uint32_t *restrict picture_width,
    uint32_t *restrict picture_height
);
static uint8_t *get_webp_argb(file_data *restrict file_data_pointer);
static void set_pixel_of_picture(
    uint64_t index,
    picture *restrict picture_pointer,
    uint8_t *restrict webp_argb
);
static WebPBitstreamFeatures *init_webp_bitstream_features();
static VP8StatusCode get_webp_info(
    file_data *restrict file_data_pointer,
    WebPBitstreamFeatures *restrict features
);
static void print_get_webp_info_error_and_exit();
static void print_animated_webp_error();

picture *read_webp_image(char *restrict file_path) {
    add_error_message_to_queue("Failed to read webp image.");
    file_data *file_data_pointer = init_file_data();
    read_file(file_data_pointer, file_path);
    picture *picture = read_webp_image_from_data(file_data_pointer);
    free_file_data(file_data_pointer);
    remove_last_error();
    return picture;
}

bool is_webp_image(char *restrict file_path) {
    add_error_message_to_queue("Cannot check if file is a webp image.");
    FILE *file_pointer = open_file(file_path, "rb");
    uint8_t *file_header = malloc(sizeof(uint8_t) * 12);
    validate_file_header(file_header);
    int size_read = fread(file_header, sizeof(uint8_t), 12, file_pointer);
    fclose(file_pointer);
    if(size_read < 12)
        return false;
    bool result = is_header_of_webp(file_header);
    remove_last_error();
    return result;
}

/**
 * Returns the picture contained in the webp file described by the given file
 * data pointer.
 * 
 * @param file_data_pointer The pointer to the content of the webp file
 *                          containing the image
 */
static picture *read_webp_image_from_data(
    file_data *restrict file_data_pointer
) {
    picture *picture_pointer = init_picture_with_webp(file_data_pointer);
    populate_picture(
        picture_pointer,
        file_data_pointer
    );
    return picture_pointer;
}

/**
 * Checks that the given file header is not null. If it is, the program will end
 * with an error message.
 *
 * @param file_header the pointer to check
 */
static void validate_file_header(uint8_t *restrict file_header) {
    if(file_header == NULL) {
        add_error_message_to_queue(
            "Failed to allocate memory to read the header of the file.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
}

/**
 * Checks that the given header of file is the header of a webp file.
 *
 * @param file_header the first twelve bytes of the file
 *
 * @return whether those are the first twelve bytes of a webp file or not.
 */
static bool is_header_of_webp(uint8_t *restrict file_header) {
    return file_header[0] == 0x52 && // R
        file_header[1] == 0x49 && // I
        file_header[2] == 0x46 && // F
        file_header[3] == 0x46 && // F
        file_header[8] == 0x57 && // W
        file_header[9] == 0x45 && // E
        file_header[10] == 0x42 && // B
        file_header[11] == 0x50; // P
}

/**
 * Initializes a picture which will have the dimension of the webp image
 * contained in the given file data pointer.
 *
 * @param file_data_pointer the pointer to the data of the file containing the
 *                          webp image
 *
 * @return a pointer to the picture that has the dimension of the given webp
 *         file
 */
static picture *init_picture_with_webp(file_data *restrict file_data_pointer) {
    uint32_t width;
    uint32_t height;
    set_width_and_height(
        file_data_pointer,
        &width,
        &height
    );
    return init_picture(width, height);
}

/**
 * Sets the argb pixels of the given picture matching the pixels of the webp
 * image contained in the given file data pointer.
 *
 * @param picture_pointer   Pointer to the picture for which to set the argb
 *                          pixels
 * @param file_data_pointer Pointer to the file data of the webp image to use
 *                          for setting the argb pixels of the picture
 */
static void populate_picture(
    picture *restrict picture_pointer,
    file_data *restrict file_data_pointer
) {
    uint8_t *result = get_webp_argb(file_data_pointer);
    for(uint64_t i=0; i<picture_pointer->width * picture_pointer->height; i++)
        set_pixel_of_picture(i, picture_pointer, result);
    free(result);
}

/**
 * Sets the given width and height to match the size of the webp image contained
 * in the given file data
 *
 * @param file_data_pointer the pointer to the file data containing the webp
 *                          image from which to extract the width and height
 * @param picture_width     the pointer to the width to set
 * @param picture_height    the pointer to the height to set
 */
static void set_width_and_height(
    file_data *restrict file_data_pointer,
    uint32_t *restrict picture_width,
    uint32_t *restrict picture_height
) {
    int width;
    int height;
    WebPBitstreamFeatures *features = init_webp_bitstream_features();
    VP8StatusCode result = get_webp_info(file_data_pointer, features);
    if(result != VP8_STATUS_OK)
        print_get_webp_info_error_and_exit();
    *picture_width = (uint32_t) features->width;
    *picture_height = (uint32_t) features->height;
    free(features);
}

/**
 * Returns a pointer to the argb pixels of the webp image contained in the
 * given file data.
 *
 * @param file_data_pointer pointer to the data of the webp image from which to
 *                          extract the ARGB pixels.
 */
static uint8_t *get_webp_argb(
    file_data *restrict file_data_pointer
) {
    return WebPDecodeARGB(
        file_data_pointer->data_pointer,
        file_data_pointer->size,
        NULL,
        NULL
    );
}

/**
 * Sets the pixel of the picture at the given index with the data of the ARGB
 * pointer of the webp image. It basically converts four 8bits into one 32bits
 * integer.
 *
 * @param index           the index of the pixel to set
 * @param picture_pointer the pointer of the picture on which to set the pixel
 * @param webp_argb       the ARGB pointer to use to set the pixel of the
 *                        picture
 */
static void set_pixel_of_picture(
    uint64_t index,
    picture *restrict picture_pointer,
    uint8_t *restrict webp_argb
) {
    uint64_t webpIndex = index*4;
    picture_pointer->argb_pixels[index] = (webp_argb[webpIndex] << 24);
    picture_pointer->argb_pixels[index] |= (webp_argb[webpIndex + 1] << 16);
    picture_pointer->argb_pixels[index] |= (webp_argb[webpIndex + 2] << 8);
    picture_pointer->argb_pixels[index] |= webp_argb[webpIndex + 3];
}

/**
 * Initializes a webp features pointer. This function will only allocate memory,
 * or end the program with an error message if allocation failed.
 *
 * @return the pointer to the webp features that has been initialized
 */
static WebPBitstreamFeatures *init_webp_bitstream_features() {
    WebPBitstreamFeatures *features = malloc(sizeof(WebPBitstreamFeatures));
    if(features == NULL) {
        add_error_message_to_queue(
            "Failed to allocate memory for config from webp file.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
    return features;
}

/**
 * Get the information of the webp image contained in the given pointer to file
 * data, using the given pointer to features to store the information.
 *
 * @param file_data_pointer the pointer to the data of the file from which to
 *                          extract the information
 * @param features          the pointer to the webp features that will be used
 *                          to store the information
 *
 * @return the status code returned by the webp library
 */
static VP8StatusCode get_webp_info(
    file_data *restrict file_data_pointer,
    WebPBitstreamFeatures *restrict features
) {
    VP8StatusCode result = WebPGetFeatures(
        file_data_pointer->data_pointer,
        file_data_pointer->size,
        features
    );
    if(features->has_animation)
        print_animated_webp_error();
    return result;
}

/**
 * Prints the error of not being able to get the configuration from the webp
 * file and exits the program.
 */
static void print_get_webp_info_error_and_exit() {
    add_error_message_to_queue("Failed to get config from webp file.\n");
    print_errors();
    exit(EXIT_FAILURE);
}

/**
 * Prints the error of unsupported animated webp and exits the program.
 */
static void print_animated_webp_error() {
    add_error_message_to_queue("Animated webp images are not supported.\n");
    print_errors();
    exit(EXIT_FAILURE);
}