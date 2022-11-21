#ifndef UTILS_ANDROID_UTILS_H
#define UTILS_ANDROID_UTILS_H

#include <stdint.h>
#include "picture_utils.h"

void write_android_files(
    picture *picture,
    uint32_t width,
    uint32_t height,
    char *output_folder,
    char *name
);

#endif  /* UTILS_ANDROID_UTILS_H */
