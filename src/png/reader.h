#ifndef PNG_READER_H
#define PNG_READER_H

#include <stdbool.h>
#include "../pictures/pictures.h"

/**
 * Returns a pointer to a picture which will represent the png picture in the
 * given file path.
 *
 * @param file_path the path to the file from which to extract the picture
 *
 * @return a pointer to the picture representing the given png image
 */
picture *read_png_image(char *restrict file_path);

/**
 * Returns whether the file at the given file path is a png image or not. It
 * will just check the first bytes of the file for the signature of a png
 * image so there is no guarantee that the file is not corrupted.
 *
 * @param file_path the path to the file to check
 *
 * @return whether the given file is a png image or not
 */
bool is_png_image(char *restrict file_path);

#endif  /* PNG_READER_H */