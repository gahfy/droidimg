#include <stdio.h>
#include <stdlib.h>
#include "../../src/pictures/pictures.h"
#include "../../src/files/reader.h"
#include "../../src/png/reader.h"
#include "../../config.h"

static void compare_pixels(picture *picture1, picture *picture2);
static void compare_size(picture *picture1, picture *picture2);
static picture *file_to_picture(
    char *restrict file_path, uint32_t width, uint32_t height
);

int main(int argc, char *argv[]) {
    #ifndef HAVE_LIBPNG
        return 77;
    #elif HAVE_LIBPNG != 1
        return 77;
    #endif
    uint32_t width = 1129;
    uint32_t height = 638;
    picture *picture_pointer = file_to_picture("img/droidbot.picture", width, height);
    picture *png_picture = read_png_image("img/droidbot.png");
    compare_pixels(picture_pointer, png_picture);
    free_picture(picture_pointer);
    free_picture(png_picture);
    return EXIT_SUCCESS;
}

static void compare_pixels(picture *picture1, picture *picture2) {
    compare_size(picture1, picture2);
    for(uint64_t i=0; i<picture1->width * picture1->height; i++) {
        if((picture1->argb_pixels[1] >> 24) != 0 || (picture2->argb_pixels[1] >> 24) != 0) {
            if(picture1->argb_pixels[i] != picture2->argb_pixels[i]) {
                exit(EXIT_FAILURE);
            }
        }
    }
}

static void compare_size(picture *picture1, picture *picture2) {
    if(
        picture1->width != picture2->width ||
        picture1->height != picture2->height
    ) {
        fprintf(stderr, "Not same size");
        exit(EXIT_FAILURE);
    }
}

static picture *file_to_picture(
    char *file_path, uint32_t width, uint32_t height
) {
    file_data *file_data_pointer = init_file_data();
    read_file(file_data_pointer, file_path);
    picture *result = init_picture(width, height);
    for(uint64_t i=0; i<width*height*4; i+=4)
        for(int j=3; j>=0; j--) {
            result->argb_pixels[i/4] = result->argb_pixels[i/4] << 8;
            result->argb_pixels[i/4] += file_data_pointer->data_pointer[i+j];
        }
    free_file_data(file_data_pointer);
    return result;
}