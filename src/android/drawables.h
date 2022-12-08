#ifndef ANDROID_DRAWABLES_H
#define ANDROID_DRAWABLES_H

#include <stdbool.h>
#include "../pictures/pictures.h"

/**
 * Type holding the configuration for generating drawables
 */
typedef struct drawable_config {
    float quality;        /**< The webp quality to apply (100.0 for lossless) */
    bool exclude_ldpi;    /**< Whether to exclude ldpi resolution or not */
    bool exclude_mdpi;    /**< Whether to exclude mdpi resolution or not */
    bool exclude_hdpi;    /**< Whether to exclude hdpi resolution or not */
    bool exclude_xhdpi;   /**< Whether to exclude xhdpi resolution or not */
    bool exclude_xxhdpi;  /**< Whether to exclude xxhdpi resolution or not */
    bool exclude_xxxhdpi; /**< Whether to exclude xxxhdpi resolution or not */
    char *name;           /**< The name of the drawable */
    uint32_t width;       /**< The width in dp of the drawable */
    uint32_t height;      /**< The height in dp of the drawable */
} drawable_config;

/**
 * Writes the given picture as android drawable, applying the given config in
 * the given resources folder.
 *
 * @param image        the pointer to the picture to add as drawable
 * @param ouput_folder the resources folder in which to write the drawables
 * @param config       the configuration to apply for generating the drawables
 */
void write_android_drawables(
    picture *restrict image,
    char *restrict output_folder,
    drawable_config *restrict config
);

#endif  /* ANDROID_DRAWABLES_H */