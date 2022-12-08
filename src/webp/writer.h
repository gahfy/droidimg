#ifndef WEBP_WRITER_H
#define WEBP_WRITER_H

#include <stdio.h>
#include "../pictures/pictures.h"

/**
 * Writes the given picture in a webp file at the given path.
 *
 * @param picture_pointer the pointer to the picture to write in the file
 * @param file_path       the path to the file to write the webp file to
 * @param width           the new width to apply to the image
 * @param height          the new height to apply to the image
 * @param quality         the quality to apply to the config (100.0 for
 *                        lossless, any number between 0.0 and 100.0 for lossy)
 */
void write_picture_to_webp(
    picture *restrict picture_pointer,
    char *restrict file_path,
    int width,
    int height,
    float quality
);

#endif  /* WEBP_WRITER_H */