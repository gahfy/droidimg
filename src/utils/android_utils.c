#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "android_utils.h"
#include "string_utils.h"
#include "webp_utils.h"
#include "memory_utils.h"

static uint32_t get_ldpi_size(uint32_t mdpi_size) {
    uint32_t size = mdpi_size * 3 / 4;
    return size == 0 ? 1 : size;
}

static uint32_t get_mdpi_size(uint32_t mdpi_size) {
    return mdpi_size == 0 ? 1 : mdpi_size;
}

static uint32_t get_hdpi_size(uint32_t mdpi_size) {
    uint32_t size = mdpi_size * 3 / 2;
    return size == 0 ? 1 : size;
}

static uint32_t get_xhdpi_size(uint32_t mdpi_size) {
    uint32_t size = mdpi_size * 2;
    return size == 0 ? 1 : size;
}

static uint32_t get_xxhdpi_size(uint32_t mdpi_size) {
    uint32_t size = mdpi_size * 3;
    return size == 0 ? 1 : size;
}

static uint32_t get_xxxhdpi_size(uint32_t mdpi_size) {
    uint32_t size = mdpi_size * 4;
    return size == 0 ? 1 : size;
}

static void write_android_picture(
    picture *picture,
    char *output_folder,
    char *drawable_folder,
    char *output_file_name,
    char *name,
    char *suffix,
    uint32_t width,
    uint32_t height
) {
    sprintf(drawable_folder, "%sdrawable-%s", output_folder, suffix);
    create_directory_if_not_exists(drawable_folder);
    sprintf(output_file_name, "%s/%s.webp", drawable_folder, name);
    write_to_webp(picture, output_file_name, width, height);
}

static void write_ldpi_picture(
    picture *picture,
    char *output_folder,
    char *drawable_folder,
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
        drawable_folder,
        output_file_name,
        name,
        "ldpi",
        get_ldpi_size(mdpi_width),
        get_ldpi_size(mdpi_height)
    );
}

static void write_mdpi_picture(
    picture *picture,
    char *output_folder,
    char *drawable_folder,
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
        drawable_folder,
        output_file_name,
        name,
        "mdpi",
        get_mdpi_size(mdpi_width),
        get_mdpi_size(mdpi_height)
    );
}

static void write_hdpi_picture(
    picture *picture,
    char *output_folder,
    char *drawable_folder,
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
        drawable_folder,
        output_file_name,
        name,
        "hdpi",
        get_hdpi_size(mdpi_width),
        get_hdpi_size(mdpi_height)
    );
}

static void write_xhdpi_picture(
    picture *picture,
    char *output_folder,
    char *drawable_folder,
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
        drawable_folder,
        output_file_name,
        name,
        "xhdpi",
        get_xhdpi_size(mdpi_width),
        get_xhdpi_size(mdpi_height)
    );
}

static void write_xxhdpi_picture(
    picture *picture,
    char *output_folder,
    char *drawable_folder,
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
        drawable_folder,
        output_file_name,
        name,
        "xxhdpi",
        get_xxhdpi_size(mdpi_width),
        get_xxhdpi_size(mdpi_height)
    );
}

static void write_xxxhdpi_picture(
    picture *picture,
    char *output_folder,
    char *drawable_folder,
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
        drawable_folder,
        output_file_name,
        name,
        "xxxhdpi",
        get_xxxhdpi_size(mdpi_width),
        get_xxxhdpi_size(mdpi_height)
    );
}

char *initialize_output_file_name(char *output_folder, char *name) {
    int output_file_max_length = strlen(output_folder);
    output_file_max_length += 17 + strlen(name) + 6;
    size_t output_file_name_size = sizeof(char) * output_file_max_length;
    char *output_file_name = allocate(output_file_name_size);
    return output_file_name;
}

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
) {
    char *output_file_name = initialize_output_file_name(output_folder, name);
    char *drawable_folder = initialize_output_file_name(output_folder, name);
    if(!exclude_ldpi) {
        printf("Converting LDPI picture: ");
        fflush(stdout);
        write_ldpi_picture(
            picture,
            output_folder,
            drawable_folder,
            output_file_name,
            name,
            width,
            height
        );
        printf("Done.\n");
    }
    if(!exclude_mdpi) {
        printf("Converting MDPI picture: ");
        fflush(stdout);
        write_mdpi_picture(
            picture,
            output_folder,
            drawable_folder,
            output_file_name,
            name,
            width,
            height
        );
        printf("Done.\n");
    }
    if(!exclude_hdpi) {
        printf("Converting HDPI picture: ");
        fflush(stdout);
        write_hdpi_picture(
            picture,
            output_folder,
            drawable_folder,
            output_file_name,
            name,
            width,
            height
        );
        printf("Done.\n");
    }
    if(!exclude_xhdpi) {
        printf("Converting XHDPI picture: ");
        fflush(stdout);
        write_xhdpi_picture(
            picture,
            output_folder,
            drawable_folder,
            output_file_name,
            name,
            width,
            height
        );
        printf("Done.\n");
    }
    if(!exclude_xxhdpi) {
        printf("Converting XXHDPI picture: ");
        fflush(stdout);
        write_xxhdpi_picture(
            picture,
            output_folder,
            drawable_folder,
            output_file_name,
            name,
            width,
            height
        );
        printf("Done.\n");
    }
    if(!exclude_xxxhdpi) {
        printf("Converting XXXHDPI picture: ");
        fflush(stdout);
        write_xxxhdpi_picture(
            picture,
            output_folder,
            drawable_folder,
            output_file_name,
            name,
            width,
            height
        );
        printf("Done.\n");
    }
    free(output_file_name);
    free(drawable_folder);
}