#ifndef WEBP_READER_H
#define WEBP_READER_H

#include <stdbool.h>
#include "../pictures/pictures.h"

/**
 * Returns a pointer to a picture which will represent the webp picture in the
 * given file path.
 *
 * @param file_path the path to the file from which to extract the picture
 *
 * @return a pointer to the picture representing the given webp image
 */
picture *read_webp_image(char *restrict file_path);

/**
 * Returns whether the file at the given file path is a webp image or not. It
 * will just check the first bytes of the file for the signature of a webp
 * image so there is no guarantee that the file is not corrupted.
 *
 * @param file_path the path to the file to check
 *
 * @return whether the given file is a webp image or not
 */
bool is_webp_image(char *restrict file_path);

#endif  /* WEBP_READER_H */