#include <errno.h>
#include <png.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "binary_utils.h"
#include "constants.h"
#include "../pictures/pictures.h"
#include "png_utils.h"
#include "memory_utils.h"
#include "../files/reader.h"
#include "../files/commons.h"

static int get_mask(int bit_depth) {
    switch(bit_depth) {
        case 1:
            return 0x01;
        case 2:
            return 0x03;
        case 4:
            return 0x0F;
        case 8:
            return 0xFF;
    }
    return -1;
}

static void set_palette_colors(
    png_struct *struct_pointer,
    png_info *info_pointer,
    png_palette *palette
) {
    int color_type = png_get_color_type(struct_pointer, info_pointer);
    if(color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(
            struct_pointer,
            info_pointer,
            &palette->colors,
            &palette->colors_size
        );
    }
}

static void set_palette_transparencies(
    png_struct *struct_pointer,
    png_info *info_pointer,
    png_palette *palette
) {
    int color_type = png_get_color_type(struct_pointer, info_pointer);
    if(color_type == PNG_COLOR_TYPE_PALETTE) {
        png_get_PLTE(
            struct_pointer,
            info_pointer,
            &palette->colors,
            &palette->colors_size
        );
    }
}

static png_definition *get_definition(
    png_struct *struct_pointer,
    png_info *info_pointer
) {
    int bit_depth = png_get_bit_depth(struct_pointer, info_pointer);
    int color_type = png_get_color_type(struct_pointer, info_pointer);
    png_palette *palette = allocate(sizeof(png_palette));
    set_palette_colors(struct_pointer, info_pointer, palette);
    set_palette_transparencies(struct_pointer, info_pointer, palette);
    png_definition *definition = allocate(sizeof(png_definition));
    definition->color_type = color_type;
    definition->bit_depth = bit_depth;
    definition->palette = palette;
    return definition;
}

static bool is_alpha(png_definition *definition) {
    return (definition->color_type & PNG_COLOR_MASK_ALPHA) != 0;
}

static bool is_color(png_definition *definition) {
    return (definition->color_type & PNG_COLOR_MASK_COLOR) != 0;
}

static bool is_palette(png_definition *definition) {
    return definition->color_type == PNG_COLOR_TYPE_PALETTE;
}

static int get_right_shift(png_definition *definition, uint32_t x) {
    int multiplier = 8 / definition->bit_depth;
    return (multiplier - 1 - x % multiplier) * definition->bit_depth;
}

static uint32_t *get_picture_pixel(
    picture *picture,
    uint32_t x,
    uint32_t y
) {
    uint64_t pixel_index = y * picture->width + x;
    return &picture->argb_pixels[pixel_index];
}

static png_byte get_transparency(png_palette *palette, int index) {
    if(index < palette->transparencies_size)
        return palette->transparencies[index];
    else
        return 0xFF;
}

static png_color get_color_from_palette(png_palette *palette, int index) {
    if(index < palette->colors_size)
        return palette->colors[index];
    else
        return (png_color) {0x00, 0x00, 0x00};
}

static bool is_gray_or_palette(png_definition *definition) {
    return definition->color_type == PNG_COLOR_TYPE_GRAY
        || definition->color_type == PNG_COLOR_TYPE_PALETTE;
}

static void set_pixel_to_picture_from_16bits_png(
    uint32_t *picture_pixel,
    png_bytep pixel,
    int color_type
) {
    uint8_t alpha_byte = 0xFF;
    if((color_type & PNG_COLOR_MASK_ALPHA) != 0)
        alpha_byte = pixel[((color_type & PNG_COLOR_MASK_COLOR) == 0) ? 2 : 6];
    int green_byte = pixel[(color_type & PNG_COLOR_MASK_COLOR) ? 2 : 0];
    int blue_byte = pixel[(color_type & PNG_COLOR_MASK_COLOR) ? 4 : 0];
    uint8_to_uint32(picture_pixel, alpha_byte, pixel[0], green_byte, blue_byte);
}

static void set_pixel_to_picture_from_gray(
    uint32_t *picture_pixel, uint8_t pixel_value, png_definition *definition
) {
    uint8_t gray_byte = 0;
    for(int i=0; i<(8 / definition->bit_depth); i++)
        gray_byte = (gray_byte << definition->bit_depth) | pixel_value;
    uint8_to_uint32(picture_pixel, 0xFF, gray_byte, gray_byte, gray_byte);
}

static void set_pixel_to_picture_from_palette(
    uint32_t *picture_pixel,
    uint8_t index,
    png_definition *definition
) {
    uint8_to_uint32(
        picture_pixel,
        get_transparency(definition->palette, index),
        get_color_from_palette(definition->palette, index).red,
        get_color_from_palette(definition->palette, index).green,
        get_color_from_palette(definition->palette, index).blue
    );
}

static void set_pixel_to_picture_from_palette_or_gray_png(
    uint32_t *picture_pixel,
    uint32_t x,
    png_bytep pixel,
    png_definition *definition
) {
    int right_shift = get_right_shift(definition, x);
    uint8_t pixel_value = (pixel[0] >> right_shift);
    pixel_value = pixel_value & get_mask(definition->bit_depth);
    if(definition->color_type == PNG_COLOR_TYPE_GRAY)
        set_pixel_to_picture_from_gray(picture_pixel, pixel_value, definition);
    else
        set_pixel_to_picture_from_palette(
            picture_pixel, pixel_value, definition
        );
}

static void set_pixel_to_picture_from_alpha_png(
    uint32_t *picture_pixel,
    png_bytep pixel,
    bool is_color_png,
    bool has_alpha_channel
) {
    uint8_t alpha = has_alpha_channel ? pixel[is_color_png ? 3 : 1] : 0xFF;
    int red = pixel[0];
    int green = pixel[is_color_png ? 1 : 0];
    int blue = pixel[is_color_png ? 2 : 0];
    uint8_to_uint32(picture_pixel, alpha, red, green, blue);
}

static void set_pixel_to_picture_from_up_to_8bits_png(
    uint32_t *picture_pixel,
    uint32_t x,
    png_bytep pixel,
    png_definition *definition
) {
    if(is_gray_or_palette(definition))
        set_pixel_to_picture_from_palette_or_gray_png(
            picture_pixel, x, pixel, definition
        );
    else
        set_pixel_to_picture_from_alpha_png(
            picture_pixel, pixel, is_color(definition), is_alpha(definition)
        );
}

static void set_pixel_to_picture_from_png(
    uint32_t *picture_pixel,
    png_bytep pixel,
    uint32_t x,
    png_definition *definition
) {
    if(definition->bit_depth == 16)
        set_pixel_to_picture_from_16bits_png(
            picture_pixel, pixel, definition->color_type
        );
    else
        set_pixel_to_picture_from_up_to_8bits_png(
            picture_pixel, x, pixel, definition
        );
}

static int get_bits_per_pixel(png_definition *definition) {
    int bits_per_pixel = definition->bit_depth;
    if(!is_palette(definition)) {
        if(is_color(definition)) {
            bits_per_pixel += definition->bit_depth * 2;
        }
        if(is_alpha(definition)) {
            bits_per_pixel += definition->bit_depth;
        }
    }
    return bits_per_pixel;
}

static void set_pixels_to_picture_from_row(
    picture *picture_pointer,
    png_byte *row_pointer,
    png_definition *definition,
    uint32_t y
) {
    for(uint32_t x=0; x<picture_pointer->width; x++) {
        uint32_t *picture_pixel = get_picture_pixel(picture_pointer, x, y);
        uint64_t pixel_index = x * get_bits_per_pixel(definition) / 8;
        png_bytep pixel = &(row_pointer[pixel_index]);
        set_pixel_to_picture_from_png(picture_pixel, pixel, x, definition);
    }
}

static void set_pixels_to_picture_from_png(
    picture *picture_pointer,
    png_byte *row_pointer,
    png_struct *struct_pointer,
    png_definition *definition
) {
    for(uint32_t y=0; y<picture_pointer->height; y++) {
        png_read_row(struct_pointer, row_pointer, NULL);
        set_pixels_to_picture_from_row(
            picture_pointer, row_pointer, definition, y
        );
    }
}

static picture *initialize_picture(
    png_struct *struct_pointer,
    png_info *info_pointer
) {
    picture *picture_pointer = allocate(sizeof(picture));
    picture_pointer->width = png_get_image_width(struct_pointer, info_pointer);
    picture_pointer->height = png_get_image_height(
        struct_pointer, info_pointer
    );
    picture_pointer->argb_pixels = allocate(
        sizeof(uint32_t) * picture_pointer->width * picture_pointer->height
    );
    return picture_pointer;
}

static png_byte *initialize_png_row(
    png_struct *struct_pointer,
    png_info *info_pointer
) {
    size_t row_pointer_size = png_get_rowbytes(struct_pointer, info_pointer);
    png_byte *row_pointer = allocate(row_pointer_size);
    return row_pointer;
}

static picture *get_picture_from_png_struct(
    png_struct *struct_pointer,
    png_info *info_pointer
) {
    picture *picture_pointer = initialize_picture(struct_pointer, info_pointer);
    png_byte *row_pointer = initialize_png_row(struct_pointer, info_pointer);
    png_definition *definition = get_definition(struct_pointer, info_pointer);
    set_pixels_to_picture_from_png(
        picture_pointer, row_pointer, struct_pointer, definition
    );
    free(definition->palette);
    free(definition);
    free(row_pointer);
    return picture_pointer;
}

static png_struct *init_struct_pointer() {
    png_struct *result = png_create_read_struct(
        PNG_LIBPNG_VER_STRING,
        NULL,
        NULL,
        NULL
    );
    if(result == NULL) {
        fprintf(stderr, ERROR_MESSAGE_MEMORY);
        exit(ERROR_CODE_MEMORY);
    }
    return result;
}

static png_info *init_info_pointer(png_struct *struct_pointer) {
    png_info *result = png_create_info_struct(struct_pointer);
    if(result == NULL) {
        fprintf(stderr, ERROR_MESSAGE_MEMORY);
        exit(ERROR_CODE_MEMORY);
    }
    return result;
}

/*
 * @brief Returns a pointer to an picture structure containing the image data of
 * the given PNG file.
 *
 * @param file_path The path to the PNG file from which to extract the picture
 *
 * @return the pointer to the structure, or NULL if an error occurred
 */
picture *get_picture_from_png_file(char *file_path) {
    FILE *fp = open_file(file_path, "rb");
    png_struct *struct_pointer = init_struct_pointer();
    png_info *info_pointer = init_info_pointer(struct_pointer);
    png_init_io(struct_pointer, fp);
    png_read_info(struct_pointer, info_pointer);
    picture *result_picture = get_picture_from_png_struct(
        struct_pointer,
        info_pointer
    );
    png_destroy_read_struct(&struct_pointer, &info_pointer, NULL);
    fclose(fp);
    return result_picture;
}
