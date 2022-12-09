#include "writer.h"
#include "../files/commons.h"
#include "../logging/logging.h"
#include <webp/encode.h>
#include <stdlib.h>
#include <stdbool.h>

static void init_struct(
    WebPConfig *restrict config_pointer,
    WebPPicture *restrict webp_picture_pointer,
    FILE *restrict file_pointer,
    uint32_t width,
    uint32_t height,
    float quality
);
static void write_pixels_to_webp_picture(
    WebPPicture *restrict webp_picture_pointer,
    picture *restrict picture_pointer,
    uint32_t width,
    uint32_t height
);
static void write_webp_picture_to_file(
    WebPConfig *restrict config_pointer,
    WebPPicture *restrict webp_picture_pointer
);
static void desallocate_memory(
    FILE *restrict file_pointer, WebPPicture *restrict webp_picture_pointer
);
static void init_webp_config(
    WebPConfig *restrict config_pointer, float quality
);
static void init_webp_picture(
    WebPPicture *restrict webp_picture_pointer,
    FILE *restrict file_pointer,
    uint32_t width,
    uint32_t height
);
static void validate_webp_config(WebPConfig *restrict config);
static void init_webp_picture_pointer_or_quit(
    WebPPicture *restrict webp_picture_pointer
);
static void allocate_webp_picture(WebPPicture *restrict webp_picture_pointer);
static int write_picture_to_file(
    const uint8_t* data,
    size_t data_size,
    const WebPPicture* picture
);

void write_picture_to_webp(
    picture *restrict picture_pointer,
    char *restrict file_path,
    int width,
    int height,
    float quality
) {
    WebPConfig config; WebPPicture webp_picture;
    FILE *file_pointer = open_file(file_path, "wb");
    init_struct(&config, &webp_picture, file_pointer, width, height, quality);
    write_pixels_to_webp_picture(&webp_picture, picture_pointer, width, height);
    write_webp_picture_to_file(&config, &webp_picture);
    desallocate_memory(file_pointer, &webp_picture);
}

/**
 * Initializes the given config and webp picture structures.
 *
 * @param config_pointer       The pointer to the config structure to initialize
 * @param webp_picture_pointer The pointer to the webp image structure to
 *                             initialize
 * @param file_pointer         The pointer to the file the webp image will be
 *                             written to
 * @param width                The width of the image that will be written
 * @param height               The height of the image that will be written
 * @param quality              The quality to apply to the config (100.0 for
 *                             lossless, any number between 0.0 and 100.0 for
 *                             lossy)
 */
static void init_struct(
    WebPConfig *restrict config_pointer,
    WebPPicture *restrict webp_picture_pointer,
    FILE *restrict file_pointer,
    uint32_t width,
    uint32_t height,
    float quality
) {
    
    init_webp_config(config_pointer, quality);
    init_webp_picture(webp_picture_pointer, file_pointer, width, height);
}

/**
 * Writes the pixels of the given picture to the given webp picture structure.
 *
 * @param webp_picture_pointer The pointer to the webp image to which the pixels
 *                             will be written
 * @param picture_pointer      The pointer to the picture containing the pixels
 *                             to write
 * @param width                The width of the new image
 * @param height               The height of the new image
 */
static void write_pixels_to_webp_picture(
    WebPPicture *restrict webp_picture_pointer,
    picture *restrict picture_pointer,
    uint32_t width,
    uint32_t height
) {
    for(uint32_t y=0; y<height; y++) {
        for(uint32_t x=0; x<width; x++) {
            uint32_t index = (y*width+x);
            webp_picture_pointer->argb[index] = get_pixel_at(
                picture_pointer, x, y, width, height
            );
        }
    }
}

/**
 * Writes the given webp picture to the file contained in its structure.
 *
 * @param config_pointer       The pointer to the configuration of the image
 * @param webp_picture_pointer The pointer to the webp image to write
 */
static void write_webp_picture_to_file(
    WebPConfig *restrict config_pointer,
    WebPPicture *restrict webp_picture_pointer
) {
    int ok = WebPEncode(config_pointer, webp_picture_pointer);
    if (!ok) {
        loge("Failed to encode webp picture.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Desallocate memory for the two given pointer.
 *
 * @param file_pointer         the pointer to the file to close
 * @param webp_picture_pointer the pointer to the webp picture to free
 */
static void desallocate_memory(
    FILE *restrict file_pointer, WebPPicture *restrict webp_picture_pointer
) {
    fclose(file_pointer);
    WebPPictureFree(webp_picture_pointer);
}

/**
 * Initializes the given webp config pointer.
 *
 * @param config_pointer the pointer to the config to initialize
 * @param quality        the quality to apply to the config (100.0 for lossless,
 *                       any number between 0.0 and 100.0 for lossy)
 */
static void init_webp_config(
    WebPConfig *restrict config_pointer, float quality
) {
    WebPConfigPreset(
        config_pointer, WEBP_PRESET_DEFAULT, quality
    );
    config_pointer->lossless = ((quality == 100.0) ? 1 : 0);
    config_pointer->method = 6;
    validate_webp_config(config_pointer);
}

/**
 * Initializes the given webp pointer with the given parameter.
 *
 * @param webp_picture_pointer the pointer to the webp picture to initialize
 * @param file_pointer         the pointer to the file to write the webp picture
 *                             to
 * @param width                the width of the webp picture
 * @param height               the height of the webp picture
 */
static void init_webp_picture(
    WebPPicture *restrict webp_picture_pointer,
    FILE *restrict file_pointer,
    uint32_t width,
    uint32_t height
) {
    init_webp_picture_pointer_or_quit(webp_picture_pointer);
    webp_picture_pointer->use_argb = true;
    webp_picture_pointer->width = width;
    webp_picture_pointer->height = height;
    allocate_webp_picture(webp_picture_pointer);
    webp_picture_pointer->writer = write_picture_to_file;
    webp_picture_pointer->custom_ptr = file_pointer;
}

/**
 * Checks that the given config is valid. If it is not, the program will end
 * with an error message.
 *
 * @param config the webp config to check
 */
static void validate_webp_config(WebPConfig *restrict config) {
    int config_error = WebPValidateConfig(config);
    if(config_error == 0) {
        loge("Webp provided configuration is not valid.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Initializes the given webp picture or ends the program with an error message
 * if it fails.
 *
 * @param webp_picture_pointer the pointer to the webp picture to initialize
 */
static void init_webp_picture_pointer_or_quit(
    WebPPicture *restrict webp_picture_pointer
) {
    if(!WebPPictureInit(webp_picture_pointer)) {
        loge("Failed to initialize webp picture.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Allocates the memory for the given webp picture. If allocation fails, the
 * program will end with an error message.
 *
 * @param webp_picture_pointer the pointer to the webp picture to allocate
 */
static void allocate_webp_picture(WebPPicture *restrict webp_picture_pointer) {
    if(!WebPPictureAlloc(webp_picture_pointer)) {
        loge("Failed to allocate memory for the webp picture.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Writes the given data (up to the given size) to the file in the given picture
 * pointer
 *
 * @param data      the data to write to the file
 * @param data_size the size of the data to write
 * @param picture   the pointer to the picture containing the file to write to
 */
static int write_picture_to_file(
    const uint8_t* data,
    size_t data_size,
    const WebPPicture* picture
) {
    FILE *file_pointer = picture->custom_ptr;
    return fwrite(data, 1, data_size, file_pointer) == data_size;
}