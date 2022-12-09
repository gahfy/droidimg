#include <stdio.h>
#include <stdlib.h>
#include "../../src/pictures/pictures.h"
#include "../../src/files/reader.h"

static picture *file_to_picture(
    char *restrict file_path, uint32_t width, uint32_t height
);

static void compare_pixels(
    picture *picture_pointer, picture *new_picture,
    uint32_t new_width, uint32_t new_height
);

int main(int argc, char *argv[]) {
    uint32_t new_width = 846;
    uint32_t new_height = 477;
    uint32_t width = 1129;
    uint32_t height = 638;
    picture *picture_pointer = file_to_picture("pictures/droidbot_1129x638.picture", width, height);
    picture *new_picture = file_to_picture("pictures/droidbot_846x477.picture", new_width, new_height);
    compare_pixels(picture_pointer, new_picture, new_width, new_height);
    free_picture(picture_pointer);
    free_picture(new_picture);
    return EXIT_SUCCESS;
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

static void compare_pixels(
    picture *picture_pointer, picture *new_picture,
    uint32_t new_width, uint32_t new_height
) {
    for(uint32_t i=0; i<new_height; i++) {
        for(uint32_t j=0; j<new_width; j++) {
            uint32_t pixel_from_file = new_picture->argb_pixels[i*new_width+j];
            uint32_t calculated_pixel = get_pixel_at(picture_pointer, j, i, new_width, new_height);
            if(pixel_from_file != calculated_pixel) {
                fprintf(stderr, "Pixels are different at %u, %u\n", j, i);
                fprintf(stderr, "%08X - %08X\n", pixel_from_file, calculated_pixel);
                exit(EXIT_FAILURE);
            }
        }
    }
}