#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include "android_utils.h"
#include "string_utils.h"
#include "webp_utils.h"
#include "memory_utils.h"

static int get_ldpi_size(int mdpi_size) {
    int size = mdpi_size * 3 / 4;
    return size == 0 ? 1 : size;
}

static int get_mdpi_size(int mdpi_size) {
    return mdpi_size == 0 ? 1 : mdpi_size;
}

static int get_hdpi_size(int mdpi_size) {
    int size = mdpi_size * 3 / 2;
    return size == 0 ? 1 : size;
}

static int get_xhdpi_size(int mdpi_size) {
    int size = mdpi_size * 2;
    return size == 0 ? 1 : size;
}

static int get_xxhdpi_size(int mdpi_size) {
    int size = mdpi_size * 3;
    return size == 0 ? 1 : size;
}

static int get_xxxhdpi_size(int mdpi_size) {
    int size = mdpi_size * 4;
    return size == 0 ? 1 : size;
}

static void write_android_picture(
    picture *picture,
    char *output_folder,
    char *output_file_name,
    char *name,
    char *suffix,
    int width,
    int height
) {
    sprintf(output_file_name, "%sdrawable-%s", output_folder, suffix);
    create_directory_if_not_exists(output_file_name);
    sprintf(output_file_name, "%s/%s.webp", output_file_name, name);
    write_to_webp(picture, output_file_name, width, height);
}

static void write_ldpi_picture(
    picture *picture,
    char *output_folder,
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
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
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
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
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
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
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
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
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
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
    char *output_file_name,
    char *name,
    uint32_t mdpi_width,
    uint32_t mdpi_height
) {
    write_android_picture(picture,
        output_folder,
        output_file_name,
        name,
        "xxxhdpi",
        get_xxxhdpi_size(mdpi_width),
        get_xxxhdpi_size(mdpi_height)
    );
}

char *initialize_output_file_name(char *output_folder, char *name) {
    int output_file_max_length = get_string_length(output_folder);
    output_file_max_length += 16 + get_string_length(name) + 6;
    size_t output_file_name_size = sizeof(char) * output_file_max_length;
    char *output_file_name = allocate(output_file_name_size);
    return output_file_name;
}

void write_android_files(
    picture *picture, uint32_t width, uint32_t height, char *output_folder,
    char *name
) {
    char *output_file_name = initialize_output_file_name(output_folder, name);
    write_ldpi_picture(
        picture, output_folder, output_file_name, name, width, height
    );
    write_mdpi_picture(picture, output_folder, output_file_name, name, width, height);
    write_hdpi_picture(picture, output_folder, output_file_name, name, width, height);
    write_xhdpi_picture(picture, output_folder, output_file_name, name, width, height);
    write_xxhdpi_picture(picture, output_folder, output_file_name, name, width, height);
    write_xxxhdpi_picture(picture, output_folder, output_file_name, name, width, height);
    free(output_file_name);
}
