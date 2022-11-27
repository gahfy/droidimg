#ifndef UTILS_ANDROID_UTILS_H
#define UTILS_ANDROID_UTILS_H

#include <stdint.h>
#include <stdbool.h>
#include "../pictures/pictures.h"

void write_android_files(
    picture *picture,
    uint32_t width,
    uint32_t height,
    char *output_folder,
    char *name,
    bool exclude_ldpi,
    bool exclude_mdpi,
    bool exclude_hdpi,
    bool exclude_xhdpi,
    bool exclude_xxhdpi,
    bool exclude_xxxhdpi
);

#endif  /* UTILS_ANDROID_UTILS_H */
