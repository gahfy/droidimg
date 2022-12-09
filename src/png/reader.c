
#include "reader.h"
#include "../../config.h"
#include "../logging/logging.h"
#include "../files/commons.h"
#if defined HAVE_LIBPNG && HAVE_LIBPNG == 1
    #include <png.h>
#endif
#include <stdio.h>
#include <stdlib.h>

static void validate_png_library();
static bool is_header_of_png(uint8_t *restrict file_header);
static void validate_file_header(uint8_t *restrict file_header);
static void print_png_library_not_found_error_and_exit();
/**
 * Reads the png image from the given file pointer and a pointer to the
 * picture matching the png image.
 *
 * @param file_pointer the file pointer to read the image from
 *
 * @return the pointer to the picture matching the png image
 */
static picture *read_png_image_from_file_pointer(
    FILE *restrict file_pointer
);

picture *read_png_image(char *restrict file_path) {
    validate_png_library();
    #if defined HAVE_LIBPNG && HAVE_LIBPNG == 1
        FILE *file_pointer = open_file(file_path, "rb");
        picture *result = read_png_image_from_file_pointer(file_pointer);
        fclose(file_pointer);
        return result;
    #endif
    return NULL;
}

bool is_png_image(char *restrict file_path) {
    FILE *file_pointer = open_file(file_path, "rb");
    uint8_t *file_header = malloc(sizeof(uint8_t) * 8);
    validate_file_header(file_header);
    int size_read = fread(file_header, sizeof(uint8_t), 8, file_pointer);
    fclose(file_pointer);
    if(size_read < 8)
        return false;
    bool result = is_header_of_png(file_header);
    return result;
}

/**
 * Checks that the given header of file is the header of a png file.
 *
 * @param file_header the first eight bytes of the file
 *
 * @return whether those are the first eight bytes of a png file or not.
 */
static bool is_header_of_png(uint8_t *restrict file_header) {
    return file_header[0] == 0x89 &&
        file_header[1] == 0x50 && // P
        file_header[2] == 0x4E && // N
        file_header[3] == 0x47 && // G
        file_header[4] == 0x0D && // \r
        file_header[5] == 0x0A && // \n
        file_header[6] == 0x1A && // 
        file_header[7] == 0x0A; // \n
}

/**
 * Checks that the given file header is not null. If it is, the program will end
 * with an error message.
 *
 * @param file_header the pointer to check
 */
static void validate_file_header(uint8_t *restrict file_header) {
    if(file_header == NULL) {
        loge("Failed to allocate memory to read the header of the file.\n");
        exit(EXIT_FAILURE);
    }
}

/**
* Validate that the PNG library is available. If it is not, then the program
* will exit with an error message.
*/
static void validate_png_library() {
    #ifndef HAVE_LIBPNG
        print_png_library_not_found_error_and_exit();
    #elif HAVE_LIBPNG != 1
        print_png_library_not_found_error_and_exit();
    #endif        
}

/**
* Prints an error message and exits the program if the png library is not
* available.
*/
static void print_png_library_not_found_error_and_exit() {
    loge("PNG images are not supported.\n");
    loge("Caused by: Package has not been compiled with --enable-png option\n");
    exit(EXIT_FAILURE);
}

#if defined HAVE_LIBPNG && HAVE_LIBPNG == 1
    /**
     * Structure which contains the palette of the PNG with a palette.
     */
    typedef struct png_palette {
        png_color *colors;        /**< Pointer to the colors of the palette */
        int colors_size;          /**< Number of colors in the palette */
        png_byte *transparencies; /**< Pointer to the alphas of the palette */
        int transparencies_size;  /**< Number of alphas in the palette */
    } png_palette;

    /**
     * Structure which contains informations used to read the PNG file.
     */
    typedef struct png_cache {
        png_struct *struct_pointer; /**< Pointer to the PNG structure */
        png_info *info_pointer;     /**< Pointer to the PNG info structure */
        int bit_depth;              /**< The bit depth of the PNG file */
        int color_type;             /**< The color type of the PNG file */
        png_palette *palette;       /**< The palette of the PNG file */
    } png_cache;

    static png_cache *init_png_cache();
    static picture *get_picture_from_png_cache(
        png_cache *restrict cache
    );
    static void free_png_cache(png_cache *restrict cache);
    static void validate_png_cache(png_cache *restrict cache);
    static void validate_png_struct_pointer(png_cache *restrict cache);
    static void validate_png_info_pointer(png_cache *restrict cache);
    static picture *initialize_picture(png_cache *restrict cache);
    static png_byte *initialize_png_row(png_cache *restrict cache);
    static void set_definition(png_cache *restrict cache);
    static void set_pixels_to_picture_from_png(
        picture *restrict picture_pointer,
        png_byte *restrict row_pointer,
        png_cache *restrict cache
    );
    static void validate_row_pointer(png_byte *restrict row_pointer);
    static void validate_palette(png_palette *restrict palette);
    static void set_palette_colors(png_cache *restrict cache);
    static void set_palette_transparencies(png_cache *restrict cache);
    static void set_pixels_to_picture_from_row(
        picture *restrict picture_pointer,
        png_byte *restrict row_pointer,
        png_cache *restrict cache,
        uint32_t y
    );
    static uint32_t *get_picture_pixel(
        picture *restrict picture,
        uint32_t x,
        uint32_t y
    );
    static int get_bits_per_pixel(png_cache *restrict cache);
    static void set_pixel_to_picture_from_png(
        uint32_t *restrict picture_pixel,
        png_byte *restrict pixel,
        uint32_t x,
        png_cache *restrict cache
    );
    static bool is_palette(png_cache *restrict cache);
    static bool is_color(png_cache *restrict cache);
    static bool is_alpha(png_cache *restrict cache);
    static void set_pixel_to_picture_from_16bits_png(
        uint32_t *restrict picture_pixel,
        png_byte *restrict pixel,
        png_cache *restrict cache
    );
    static void set_pixel_to_picture_from_up_to_8bits_png(
        uint32_t *restrict picture_pixel,
        uint32_t x,
        png_byte *restrict pixel,
        png_cache *restrict cache
    );
    static bool is_gray_or_palette(png_cache *restrict cache);
    static void set_pixel_to_picture_from_palette_or_gray_png(
        uint32_t *restrict picture_pixel,
        uint32_t x,
        png_byte *restrict pixel,
        png_cache *restrict cache
    );
    static void set_pixel_to_picture_from_alpha_or_color_png(
        uint32_t *restrict picture_pixel,
        png_byte *restrict pixel,
        bool is_color_png,
        bool has_alpha_channel
    );
    static int get_right_shift(png_cache *restrict cache, uint32_t x);
    static int get_mask(int bit_depth);
    static void set_pixel_to_picture_from_gray(
        uint32_t *restrict picture_pixel,
        uint8_t pixel_value,
        png_cache *restrict cache
    );
    static void set_pixel_to_picture_from_palette(
        uint32_t *restrict picture_pixel,
        uint8_t index,
        png_cache *restrict cache
    );
    static png_byte get_transparency(png_palette *restrict palette, int index);
    static png_color get_color_from_palette(
        png_palette *restrict palette, int index
        );

    static picture *read_png_image_from_file_pointer(
        FILE *restrict file_pointer
    ) {
        png_cache *cache = init_png_cache();
        png_init_io(cache->struct_pointer, file_pointer);
        png_read_info(
            cache->struct_pointer,
            cache->info_pointer
        );
        picture *result_picture = get_picture_from_png_cache(
            cache
        );
        free_png_cache(cache);
        return result_picture;
    }

    /**
     * Initializes a PNG cache and returns it. The program will end with an
     * error message if allocation of memory failed.
     *
     * @return the initialized PNG cache
     */
    static png_cache *init_png_cache() {
        png_cache *cache = malloc(sizeof(png_cache));
        validate_png_cache(cache);
        cache->struct_pointer = png_create_read_struct(
            PNG_LIBPNG_VER_STRING, NULL, NULL, NULL
        );
        validate_png_struct_pointer(cache);
        cache->info_pointer = png_create_info_struct(
            cache->struct_pointer
        );
        validate_png_info_pointer(cache);
        return cache;
    }

    /**
     * Returns a picture matching the png image from the given PNG cache.
     *
     * @param png_cache the PNG cache to get the picture from
     *
     * @return the picture matching the png image.
     */
    static picture *get_picture_from_png_cache(
        png_cache *restrict cache
    ) {
        picture *picture_pointer = initialize_picture(cache);
        png_byte *row_pointer = initialize_png_row(cache);
        set_definition(cache);
        set_pixels_to_picture_from_png(
            picture_pointer, row_pointer, cache
        );
        free(row_pointer);
        return picture_pointer;
    }

    /**
     * Desallocates the memory used by the given PNG cache.
     *
     * @param cache the PNG cache to desallocate
     */
    static void free_png_cache(png_cache *restrict cache) {
        if(cache != NULL) {
            png_destroy_read_struct(
                &cache->struct_pointer,
                &cache->info_pointer,
                NULL
            );
            free(cache);
        }
    }

    /**
     * Checks that the given cache is not NULL. If it is, then the program will
     * end with an error message.
     *
     * @param cache the cache to check
     */
    static void validate_png_cache(png_cache *restrict cache) {
        if(cache == NULL) {
            loge("Failed to allocate memory to read the png image.\n");
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Checks that the struct pointer in the given cache is valid. If it is not,
     * then the program will end with an error message.
     *
     * @param cache the cache containing the png struct to check
     */
    static void validate_png_struct_pointer(png_cache *restrict cache) {
        if(!cache->struct_pointer) {
            free_png_cache(cache);
            loge("Failed to create the png read struct.\n");
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Checks that the info pointer in the given cache is valid. If it is not,
     * then the program will end with an error message.
     *
     * @param cache the cache containing the png info to check
     */
    static void validate_png_info_pointer(png_cache *restrict cache) {
        if(!cache->info_pointer) {
            free_png_cache(cache);
            loge("Failed to create the png info struct.\n");
            exit(EXIT_FAILURE);
        }
    }
    
    /**
     * Initializes a picture matching the given png cache.
     *
     * @param cache the png cache to get the picture from
     *
     * @return the picture matching the png cache
     */
    static picture *initialize_picture(png_cache *restrict cache) {
        png_uint_32 width, height;
        png_get_IHDR(
            cache->struct_pointer,
            cache->info_pointer,
            &width,
            &height,
            &cache->bit_depth,
            &cache->color_type,
            NULL,
            NULL,
            NULL
        );
        return init_picture(width, height);
    }

    /**
     * Initializes a png row with the size of the width of the PNG picture in
     * the given PNG cache.
     *
     * @param cache the png cache to get the png row from
     *
     * @return the png row with a size of the width of the given PNG.
     */
    static png_byte *initialize_png_row(png_cache *restrict cache) {
        size_t pointer_size = png_get_rowbytes(
            cache->struct_pointer, cache->info_pointer
        );
        png_byte *row_pointer = malloc(pointer_size);
        validate_row_pointer(row_pointer);
        return row_pointer;
    }

    /**
     * Sets the palette of the given cache with the information already stored
     * in it.
     *
     * @param cache the cache from which to get the information of the palette
     */
    static void set_definition(png_cache *restrict cache) {
        png_palette *palette = malloc(sizeof(png_palette));
        validate_palette(palette);
        cache->palette = palette;
        set_palette_colors(cache);
        set_palette_transparencies(cache);
    }

    /**
     * Sets the pixels of the given picture with the pixels of the given png.
     *
     * @param picture_pointer the pointer to the picture to set the pixels to
     * @param row_pointer     the pointer to the buffer that will be used to
     *                        store the pixels of the picture
     * @param cache           the png cache to get the pixels from
     */
    static void set_pixels_to_picture_from_png(
        picture *restrict picture_pointer,
        png_byte *restrict row_pointer,
        png_cache *restrict cache
    ) {
        for(uint32_t y=0; y<picture_pointer->height; y++) {
            png_read_row(
                cache->struct_pointer,
                row_pointer,
                NULL
            );
            set_pixels_to_picture_from_row(
                picture_pointer, row_pointer, cache, y
            );
        }
    }

    /**
     * Checks that the given row pointer is not null. If it is, then the program
     * will end with an error message.
     *
     * @param row_pointer the row pointer to check
     */    
    static void validate_row_pointer(png_byte *restrict row_pointer) {
        if(row_pointer == NULL) {
            loge("Failed to allocate memory to store the rows of the image.\n");
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Checks that the given palette is not null. If it is, then the program
     * will end with an error message.
     *
     * @param palette the palette to check
     */
    static void validate_palette(png_palette *restrict palette) {
        if(palette == NULL) {
            loge(
                "Failed to allocate memory to store the palette of the image.\n"
            );
            exit(EXIT_FAILURE);
        }
    }

    /**
     * Sets the color of the palette in the given cache with the informations
     * which are already stored in that cache.
     *
     * @param cache the cache to which to set the colors of the palette.
     */
    static void set_palette_colors(
        png_cache *restrict cache
    ) {
        if(cache->color_type == PNG_COLOR_TYPE_PALETTE) {
            png_get_PLTE(
                cache->struct_pointer,
                cache->info_pointer,
                &cache->palette->colors,
                &cache->palette->colors_size
            );
        }
    }

    /**
     * Sets the transparencies in the given cache with the informations which
     * are already stored in that cache.
     *
     * @param cache the cache to which to set the transparencies of the palette.
     */
    static void set_palette_transparencies(png_cache *restrict cache) {
        if(cache->color_type == PNG_COLOR_TYPE_PALETTE) {
            png_get_tRNS(
                cache->struct_pointer,
                cache->info_pointer,
                &cache->palette->transparencies,
                &cache->palette->transparencies_size,
                NULL
            );
        }
    }

    /**
     * Sets the pixels of the given row in the given picture.
     *
     * @param picture_pointer the pointer to the picture to set the pixels to
     * @param row_pointer     the pointer to the row containing the pixels
     * @param cache           the png cache to get the informations from
     * @param y               the y coordinate of the row of the pixels to set
     */
    static void set_pixels_to_picture_from_row(
        picture *restrict picture_pointer,
        png_byte *restrict row_pointer,
        png_cache *restrict cache,
        uint32_t y
    ) {
        for(uint32_t x=0; x<picture_pointer->width; x++) {
            uint32_t *picture_pixel = get_picture_pixel(picture_pointer, x, y);
            uint64_t pixel_index = x * get_bits_per_pixel(cache) / 8;
            png_byte *pixel = &(row_pointer[pixel_index]);
            set_pixel_to_picture_from_png(picture_pixel, pixel, x, cache);
        }
    }

    /**
     * Returns the pointer to the pixel at the given coordinates in the given
     * picture.
     *
     * @param  picture the picture to get the pixel from
     * @param  x       the x coordinate of the pixel to get
     * @param  y       the y coordinate of the pixel to get
     * @return         the pointer to the pixel at the given coordinates in the
     *                 given picture
     */
    static uint32_t *get_picture_pixel(
        picture *restrict picture,
        uint32_t x,
        uint32_t y
    ) {
        uint64_t pixel_index = y * picture->width + x;
        return &picture->argb_pixels[pixel_index];
    }

    /**
     * Returns the number of bits used per pixel in the PNG image stored in the
     * given cache.
     *
     * @param cache the cache from which to get this information
     *
     * @return the number of bits used per pixel in the PNG image
     */
    static int get_bits_per_pixel(png_cache *restrict cache) {
        int bits_per_pixel = cache->bit_depth;
        if(!is_palette(cache)) {
            if(is_color(cache)) {
                bits_per_pixel += cache->bit_depth * 2;
            }
            if(is_alpha(cache)) {
                bits_per_pixel += cache->bit_depth;
            }
        }
        return bits_per_pixel;
    }

    /**
     * Sets the pixel to the given pointer of the picture.
     *
     * @param picture_pixel the pointer to the pixel of the picture to set
     * @param pixel         the pointer to the pixel to copy
     * @param x             the x coordinate of the pixel to set
     * @param cache         the cache storing the information of the PNG
     */
    static void set_pixel_to_picture_from_png(
        uint32_t *restrict picture_pixel,
        png_byte *restrict pixel,
        uint32_t x,
        png_cache *restrict cache
    ) {
        if(cache->bit_depth == 16)
            set_pixel_to_picture_from_16bits_png(
                picture_pixel, pixel, cache
            );
        else
            set_pixel_to_picture_from_up_to_8bits_png(
                picture_pixel, x, pixel, cache
            );
    }

    /**
     * Returns whether the PNG in the given cache uses a palette or not.
     *
     * @param cache the cache from which to get this information
     *
     * @return whether the PNG in the given cache uses a palette or not
     */
    static bool is_palette(png_cache *restrict cache) {
        return cache->color_type == PNG_COLOR_TYPE_PALETTE;
    }

    /**
     * Returns whether the PNG in the given cache uses color or not.
     *
     * @param cache the cache from which to get this information
     *
     * @return whether the PNG in the given cache uses color or not
     */
    static bool is_color(png_cache *restrict cache) {
        return (cache->color_type & PNG_COLOR_MASK_COLOR) != 0;
    }

    /**
     * Returns whether the PNG in the given cache uses alpha or not.
     *
     * @param cache the cache from which to get this information
     *
     * @return whether the PNG in the given cache uses alpha or not
     */
    static bool is_alpha(png_cache *restrict cache) {
        return (cache->color_type & PNG_COLOR_MASK_ALPHA) != 0;
    }

    /**
     * Sets the value of the pixel at the given pointer from the given picture
     * pixel encoded on 16 bits.
     *
     * @param picture_pixel the pointer to the pixel of the picture to set
     * @param pixel         the pointer to the pixel to copy
     * @param cache         the cache storing the information of the PNG
     */
    static void set_pixel_to_picture_from_16bits_png(
        uint32_t *restrict picture_pixel,
        png_byte *restrict pixel,
        png_cache *restrict cache
    ) {
        uint8_t alpha_byte = 0xFF;
        if((cache->color_type & PNG_COLOR_MASK_ALPHA) != 0)
            alpha_byte = pixel[is_color(cache) ? 2 : 6];
        int green_byte = pixel[is_color(cache) ? 2 : 0];
        int blue_byte = pixel[is_color(cache) ? 4 : 0];
        picture_pixel[0] = alpha_byte << 24;
        picture_pixel[0] = picture_pixel[0] | (pixel[0] << 16);
        picture_pixel[0] = picture_pixel[0] | (green_byte << 8);
        picture_pixel[0] = picture_pixel[0] | blue_byte;
    }

    /**
     * Sets the value of the pixel at the given pointer from the given picture
     * pixel encoded on up to 8 bits.
     *
     * @param picture_pixel the pointer to the pixel of the picture to set
     * @param x             the x coordinate of the pixel to set
     * @param pixel         the pointer to the pixel to copy
     * @param cache         the cache storing the information of the PNG
     */
    static void set_pixel_to_picture_from_up_to_8bits_png(
        uint32_t *restrict picture_pixel,
        uint32_t x,
        png_byte *restrict pixel,
        png_cache *restrict cache
    ) {
        if(is_gray_or_palette(cache))
            set_pixel_to_picture_from_palette_or_gray_png(
                picture_pixel, x, pixel, cache
            );
        else
            set_pixel_to_picture_from_alpha_or_color_png(
                picture_pixel, pixel, is_color(cache), is_alpha(cache)
            );
    }

    /**
     * Returns whether the PNG stored in the given cache is in grayscale or a
     * palette or not.
     *
     * @param cache the cache from which to get this information
     *
     * @return whether the PNG image is grayscale or palette or not
     */
    static bool is_gray_or_palette(png_cache *restrict cache) {
        return cache->color_type == PNG_COLOR_TYPE_GRAY
            || cache->color_type == PNG_COLOR_TYPE_PALETTE;
    }

    /**
     * Sets the value of the pixel at the given pointer from the given picture
     * which is a grayscale (without alpha) or a palette PNG.
     *
     * @param picture_pixel the pointer to the pixel of the picture to set
     * @param x             the x coordinate of the pixel to set
     * @param pixel         the pointer to the pixel to copy
     * @param cache         the cache storing the information of the PNG
     */
    static void set_pixel_to_picture_from_palette_or_gray_png(
        uint32_t *restrict picture_pixel,
        uint32_t x,
        png_byte *restrict pixel,
        png_cache *restrict cache
    ) {
        int right_shift = get_right_shift(cache, x);
        uint8_t pixel_value = (pixel[0] >> right_shift);
        pixel_value = pixel_value & get_mask(cache->bit_depth);
        if(cache->color_type == PNG_COLOR_TYPE_GRAY)
            set_pixel_to_picture_from_gray(picture_pixel, pixel_value, cache);
        else
            set_pixel_to_picture_from_palette(
                picture_pixel, pixel_value, cache
            );
    }

    /**
     * Sets the value of the pixel at the given pointer from the given picture
     * which is an alpha and/or color PNG.
     *
     * @param picture_pixel     the pointer to the pixel of the picture to set
     * @param pixel             the pointer to the pixel to copy
     * @param is_color_png      whether the PNG is a color PNG or not
     * @param has_alpha_channel whether the PNG has an alpha channel or not
     */
    static void set_pixel_to_picture_from_alpha_or_color_png(
        uint32_t *restrict picture_pixel,
        png_byte *restrict pixel,
        bool is_color_png,
        bool has_alpha_channel
    ) {
        uint8_t alpha = has_alpha_channel ? pixel[is_color_png ? 3 : 1] : 0xFF;
        int red = pixel[0];
        int green = pixel[is_color_png ? 1 : 0];
        int blue = pixel[is_color_png ? 2 : 0];
        picture_pixel[0] = alpha << 24;
        picture_pixel[0] = picture_pixel[0] | red << 16;
        picture_pixel[0] = picture_pixel[0] | green << 8;
        picture_pixel[0] = picture_pixel[0] | blue;
    }

    /**
     * Returns the right shift for colors which are encoded on less than 8 bits.
     *
     * @param cache the cache storing the information of the PNG
     *​ @param x     the x coordinate to calculate the right shift for
     * 
     * @return the right shift for the given x coordinate
     */
    static int get_right_shift(png_cache *restrict cache, uint32_t x) {
        int multiplier = 8 / cache->bit_depth;
        return (multiplier - 1 - x % multiplier) * cache->bit_depth;
    }

    /**
     * Returns the mask for the given bit depth.
     *
     * @param bit_depth the bit depth to get the mask for
     *
     * @return the mask for the given bit depth
     */
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

    /**
     * Sets the value of the pixel at the given pointer from the given picture
     * which is a grayscale PNG.
     *
     * @param picture_pixel the pointer to the pixel of the picture to set
     * @param pixel_value   the value of the pixel to set
     * @param cache         the cache storing the information of the PNG
     */
    static void set_pixel_to_picture_from_gray(
        uint32_t *restrict picture_pixel,
        uint8_t pixel_value,
        png_cache *restrict cache
    ) {
        uint8_t gray_byte = 0;
        for(int i=0; i<(8 / cache->bit_depth); i++)
            gray_byte = (gray_byte << cache->bit_depth) | pixel_value;
        picture_pixel[0] = 0xFF000000;
        picture_pixel[0] = picture_pixel[0] | gray_byte << 16;
        picture_pixel[0] = picture_pixel[0] | gray_byte << 8;
        picture_pixel[0] = picture_pixel[0] | gray_byte;
    }

    /**
     * Sets the value of the pixel at the given pointer from the given picture
     * which is a palette PNG.
     *
     * @param picture_pixel the pointer to the pixel of the picture to set
     * @param index         the index of the color to set
     * @param cache         the cache storing the information of the PNG
     */
    static void set_pixel_to_picture_from_palette(
        uint32_t *restrict picture_pixel,
        uint8_t index,
        png_cache *restrict cache
    ) {
        uint8_t alpha = get_transparency(cache->palette, index);
        uint8_t red = get_color_from_palette(cache->palette, index).red;
        uint8_t green = get_color_from_palette(cache->palette, index).green;
        uint8_t blue = get_color_from_palette(cache->palette, index).blue;
        picture_pixel[0] = alpha << 24;
        picture_pixel[0] = picture_pixel[0] | red << 16;
        picture_pixel[0] = picture_pixel[0] | green << 8;
        picture_pixel[0] = picture_pixel[0] | blue;
    }

    /**
     * Returns the transparency of the color at the given index in the given
     * palette.
     *
     * @param palette the palette to get the transparency from
     * @param index   the index of the color to get the transparency from
     *
     * @return the transparency of the color at the given index in the given
     *         palette
     */
    static png_byte get_transparency(png_palette *restrict palette, int index) {
        if(index < palette->transparencies_size)
            return palette->transparencies[index];
        else
            return 0xFF;
    }

    /**
     * Returns the color at the given index in the given palette.
     *
     * @param palette the palette to get the color from
     * @param index   the index of the color to get
     *
     * @return the color at the given index in the given palette
     */
    static png_color get_color_from_palette(
        png_palette *restrict palette,
        int index
    ) {
        if(index < palette->colors_size)
            return palette->colors[index];
        else
            return (png_color) {0x00, 0x00, 0x00};
    }
#else
    static picture *read_png_image_from_file_pointer(
        FILE *restrict file_pointer
    ) {
        print_png_library_not_found_error_and_exit();
        return init_picture(1, 1);
    }
#endif