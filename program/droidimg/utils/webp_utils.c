#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <webp/encode.h>
#include "constants.h"
#include "picture_utils.h"
#include "memory_utils.h"

static void validate_webp_config(
    WebPConfig *config
) {
    int config_error = WebPValidateConfig(config);
    if(config_error == 0) {
        fprintf(stderr, ERROR_MESSAGE_WEBP_CONFIG);
        exit(ERROR_CODE_WEBP_CONFIG);
    }
}

static void init_webp_config(WebPConfig *config_pointer) {
    WebPConfigPreset(config_pointer, WEBP_PRESET_DEFAULT, 100.0);
    config_pointer->lossless = 1;
    config_pointer->method = 6;
    validate_webp_config(config_pointer);
}

static void init_webp_picture_pointer_or_quit(
    WebPPicture *webp_picture_pointer
) {
    if(!WebPPictureInit(webp_picture_pointer)) {
        fprintf(stderr, ERROR_MESSAGE_WEBP_PICTURE);
        exit(ERROR_CODE_WEBP_PICTURE);
    }
}

static void allocate_webp_picture(WebPPicture *webp_picture_pointer) {
    if(!WebPPictureAlloc(webp_picture_pointer)) {
        fprintf(stderr, ERROR_MESSAGE_MEMORY);
        exit(ERROR_CODE_MEMORY);
    }
}

static void init_webp_picture(
    WebPPicture *webp_picture_pointer,
    WebPMemoryWriter *writer_pointer,
    uint32_t width,
    uint32_t height
) {
    init_webp_picture_pointer_or_quit(webp_picture_pointer);
    webp_picture_pointer->use_argb = true;
    webp_picture_pointer->width = width;
    webp_picture_pointer->height = height;
    allocate_webp_picture(webp_picture_pointer);
    webp_picture_pointer->writer = WebPMemoryWrite;
    webp_picture_pointer->custom_ptr = writer_pointer;
}

static void init_webp_structures(
    WebPConfig *config_pointer,
    WebPMemoryWriter *writer_pointer,
    WebPPicture *webp_picture_pointer,
    uint32_t width,
    uint32_t height
) {
    init_webp_config(config_pointer);
    WebPMemoryWriterInit(writer_pointer);
    init_webp_picture(webp_picture_pointer, writer_pointer, width, height);
}

static void write_pixels_to_webp_picture(
    WebPPicture *webp_picture_pointer,
    picture *picture_pointer,
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

static void write_webp_picture_to_file(
    char *file_path,
    WebPConfig *config_pointer,
    WebPMemoryWriter *writer_pointer,
    WebPPicture *webp_picture_pointer
) {
    FILE *file_pointer = open_file(file_path, "wb");
    int ok = WebPEncode(config_pointer, webp_picture_pointer);
    if (!ok) {
        fprintf(stderr, ERROR_MESSAGE_WEBP_ENCODING);
        exit(ERROR_CODE_WEBP_ENCODING);
    }
    fwrite(writer_pointer->mem, 1, writer_pointer->size, file_pointer);
    fclose(file_pointer);
}

void write_to_webp(
    picture *picture_pointer,
    char *file_path,
    uint32_t width,
    uint32_t height
) {
    WebPConfig config;
    WebPMemoryWriter writer;
    WebPPicture webp_picture;
    init_webp_structures(&config, &writer, &webp_picture, width, height);
    write_pixels_to_webp_picture(&webp_picture, picture_pointer, width, height);
    write_webp_picture_to_file(file_path, &config, &writer, &webp_picture);
    WebPMemoryWriterClear(&writer);
    WebPPictureFree(&webp_picture);
}
