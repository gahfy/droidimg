#ifndef UTILS_PNG_UTILS_H
#define UTILS_PNG_UTILS_H

#include "../pictures/pictures.h"

typedef struct png_palette {
    png_color *colors;
    int colors_size;
    png_byte *transparencies;
    int transparencies_size;
 } png_palette;

 typedef struct png_definition {
    int color_type;
    int bit_depth;
    png_palette *palette;
 } png_definition;

picture *get_picture_from_png_file(char *file_path);

#endif  /* UTILS_PNG_UTILS_H */
