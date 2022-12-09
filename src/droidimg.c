#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <webp/encode.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include "png/reader.h"
#include "webp/reader.h"
#include "webp/writer.h"
#include "pictures/pictures.h"
#include "utils/config_utils.h"
#include "logging/logging.h"
#include "android/drawables.h"

static char *input_file = NULL;
static char *output_folder = NULL;
static char *destination = NULL;
static char *name = NULL;
static uint32_t width = UINT32_MAX;
static uint32_t height = UINT32_MAX;
static bool exclude_ldpi = false;
static bool exclude_mdpi = false;
static bool exclude_hdpi = false;
static bool exclude_xhdpi = false;
static bool exclude_xxhdpi = false;
static bool exclude_xxxhdpi = false;

static void usage() {
    printf("usage: droidimg <input_png_file> ");
    printf("[-o|--output <output_resource_folder>]\n");
    printf("                [-n|--name <file_name>] [-w|--with <width_in_dp>]\n");
    printf("                [-h|--height <height_in_dp>]\n");
    printf("                [-d|--destination <destination_from_config>]\n");
    printf("                [-e|--exclude <l,m,h,x,xx,xxx>]\n");
}

static uint32_t get_uint32_arg(char *argument) {
    char *end;
    unsigned long result = strtoul(argument, &end, 10);
    if(end[0] != 0x00 || result > UINT32_MAX) {
        return UINT32_MAX;
    }
    return (uint32_t) result;
}

static int get_last_slash(char *restrict string) {
    char slash = '/';
    char slash2 = '/';
    #ifdef _WIN32
        char slash2 = '\\';
    #endif
    int current_index = 0;
    int result = -1;
    while(string[current_index] != 0x00) {
        if(string[current_index] == slash || string[current_index] == slash2)
            result = current_index;
        current_index++;
    }
    return result;
}

static int get_last_point(char *restrict string, int start_index) {
    int current_index = start_index;
    int result = -1;
    while(string[current_index] != 0x00) {
        if(string[current_index] == '.')
            result = current_index;
        current_index++;
    }
    return result;
}

static void validate_output_folder() {
    if(output_folder == NULL) {
        loge("Failed to allocate memory for output folder argument.\n");
        exit(EXIT_FAILURE);
    }
}

static void parse_input_file(int argc, char *argv[], int *index) {
    input_file = malloc(sizeof(char) * (strlen(argv[index[0]]) + 1));
    if(input_file == NULL) {
        loge("Failed to allocate memory for input file argument.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(input_file, argv[index[0]], strlen(argv[index[0]])+1);
    index[0]++;
}

static void parse_output_folder(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--output") * strcmp(argv[index[0]], "-o") == 0) {
        // +2 because we may add a final / or \ at the end of the name
        output_folder = malloc(sizeof(char) * (strlen(argv[index[0]+1]) + 2));
        validate_output_folder();
        memcpy(output_folder, argv[index[0]+1], strlen(argv[index[0]+1])+1);
        index[0] += 2;
    }
}

static void parse_name(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--name") * strcmp(argv[index[0]], "-n") == 0) {
        name = malloc(sizeof(char) * (strlen(argv[index[0]+1]) + 1));
        if(name == NULL) {
            loge("Failed to allocate memory for name argument.\n");
            exit(EXIT_FAILURE);
        }
        memcpy(name, argv[index[0]+1], strlen(argv[index[0]+1])+1);
        index[0] += 2;
    }
}

static void parse_width(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--width") * strcmp(argv[index[0]], "-w") == 0) {
        width = get_uint32_arg(argv[index[0]+1]);
        index[0] += 2;
    }
}

static void parse_height(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--height") * strcmp(argv[index[0]], "-h") == 0) {
        height = get_uint32_arg(argv[index[0]+1]);
        index[0] += 2;
    }
}

static void parse_destination(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(
          strcmp(argv[index[0]], "--destination")
        * strcmp(argv[index[0]], "-d") == 0
    ) {
        destination = malloc(sizeof(char) * (strlen(argv[index[0]+1]) + 1));
        if(destination == NULL) {
            loge("Failed to allocate memory for destination argument.\n");
            exit(EXIT_FAILURE);
        }
        memcpy(destination, argv[index[0]+1], strlen(argv[index[0]+1])+1);
        index[0] += 2;
    }
}

static void parse_exclude(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--exclude") * strcmp(argv[index[0]], "-e") == 0) {
        int current = 0;
        while(argv[index[0]+1][current] != 0x00) {
            if(
                   argv[index[0]+1][current] == 'l'
                && (   argv[index[0]+1][current+1] == ','
                    || argv[index[0]+1][current+1] == 0x00
                   )
            ) {
                exclude_ldpi = true;
                current += (argv[index[0]+1][current+1] == 0x00) ? 1 : 2;
            } else if(
                          argv[index[0]+1][current] == 'm'
                       && (
                              argv[index[0]+1][current+1] == ','
                           || argv[index[0]+1][current+1] == 0x00
                          )
            ) {
                exclude_mdpi = true;
                current += (argv[index[0]+1][current+1] == 0x00) ? 1 : 2;
            } else if(
                         argv[index[0]+1][current] == 'h'
                      && (
                             argv[index[0]+1][current+1] == ','
                          || argv[index[0]+1][current+1] == 0x00
                         )
            ) {
                exclude_hdpi = true;
                current += (argv[index[0]+1][current+1] == 0x00) ? 1 : 2;
            } else if(
                         argv[index[0]+1][current] == 'x'
                      && (
                             argv[index[0]+1][current+1] == ','
                          || argv[index[0]+1][current+1] == 0x00
                         )
            ) {
                exclude_xhdpi = true;
                current += (argv[index[0]+1][current+1] == 0x00) ? 1 : 2;
            } else if(
                         argv[index[0]+1][current] == 'x'
                      && argv[index[0]+1][current+1] == 'x'
                      && (
                             argv[index[0]+1][current+2] == ','
                          || argv[index[0]+1][current+2] == 0x00
                         )
            ) {
                exclude_xxhdpi = true;
                current += (argv[index[0]+1][current+2] == 0x00) ? 2 : 3;
            } else if(
                         argv[index[0]+1][current] == 'x'
                      && argv[index[0]+1][current+1] == 'x'
                      && argv[index[0]+1][current+2] == 'x'
                      && (
                             argv[index[0]+1][current+3] == ','
                          || argv[index[0]+1][current+3] == 0x00
                         )
            ) {
                exclude_xxxhdpi = true;
                current += (argv[index[0]+1][current+3] == 0x00) ? 3 : 4;
            } else {
                usage();
                exit(EXIT_FAILURE);
            }
        }
        index[0] += 2;
    }
}

static void parse_arguments(int argc, char *argv[]) {
    int index = 1;
    int previous_index = 1;
    while(index < argc) {
        parse_output_folder(argc, argv, &index);
        parse_name(argc, argv, &index);
        parse_width(argc, argv, &index);
        parse_height(argc, argv, &index);
        parse_destination(argc, argv, &index);
        parse_exclude(argc, argv, &index);
        if(index == previous_index && input_file == NULL) {
            parse_input_file(argc, argv, &index);
        }
        if(index == previous_index) {
            usage();
            exit(EXIT_FAILURE);
        }
        previous_index = index;
    }
}

static void set_output_folder_from_null_destination() {
    output_folder = malloc(3 * sizeof(char));
    validate_output_folder();
    sprintf(output_folder, "./");
}

static void set_output_folder_from_destination() {
    if(destination == NULL) {
        set_output_folder_from_null_destination();
    } else {
        // Allocate the max length of a folder
        output_folder = malloc(sizeof(char) * 4097);
        validate_output_folder();
        set_destination(destination, output_folder);
        if(output_folder[0] == 0x00) {
            free(output_folder);
            set_output_folder_from_null_destination();
        }
        free(destination);
    }
}

static void set_output_folder_value() {
    if(output_folder == NULL) {
        set_output_folder_from_destination();
    }
    if(   output_folder[strlen(output_folder) - 1] != '/'
        && output_folder[strlen(output_folder) - 1] != '\\') {
        #ifdef _WIN32
            char char_to_add = '\\';
        #else
            char char_to_add = '/';
        #endif
        int index = strlen(output_folder);
        if(index == 4096) {
            index -= 1;
        }
        output_folder[index++] = char_to_add;
        output_folder[index] = 0x00;
    }
}

static void set_name_from_input_file() {
    if(name == NULL) {
        int start_index = get_last_slash(input_file);
        start_index = start_index == -1 ? 0 : start_index + 1;
        int end_index = get_last_point(input_file, start_index);
        if (end_index == -1)
            end_index = strlen(input_file) - 1;

        int length = end_index - start_index;
        name = malloc(sizeof(char) * (length + 1));
        if(name == NULL) {
            loge("Failed to allocate memory for name argument.\n");
            exit(EXIT_FAILURE);
        }
        memcpy(name, &input_file[start_index], length+1);
    }
}

static void set_width_and_height(picture *picture) {
    if(width == UINT32_MAX || height == UINT32_MAX) {
        if(width == UINT32_MAX && height == UINT32_MAX) {
            width = picture->width / 4;
            height = picture->height / 4;
        } else if(width == UINT32_MAX) {
            uint64_t result = ((uint64_t) picture->width) * (uint64_t) height;
            result /= (uint64_t) picture->height;
            if(result > UINT32_MAX / 4) {
                loge("Wrong width and/or height passed to program.\n");
                exit(EXIT_FAILURE);
            }
            width = (uint32_t) result;
        } else if(height == UINT32_MAX) {
            uint64_t result = ((uint64_t) picture->height) * (uint64_t) width;
            result /= (uint64_t) picture->width;
            if(result > UINT32_MAX / 4) {
                loge("Wrong width and/or height passed to program.\n");
                exit(EXIT_FAILURE);
            }
            height = (uint32_t) result;;
        }
    }
}

static void validate_arguments() {
    // If width or height is higher than max / 4, then we won't be able to
    // generate xxxhdpi (which is the size * 4)
    if(
        input_file == NULL ||
        (width != UINT32_MAX && width > UINT32_MAX / 4) ||
        (height != UINT32_MAX && height > UINT32_MAX / 4)
    ) {
        loge("Wrong arguments passed to program.\n");
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[]) {
    parse_arguments(argc, argv);
    validate_arguments();
    set_output_folder_value();
    set_name_from_input_file();
    picture *picture_pointer;
    if(is_webp_image(input_file)) {
        picture_pointer = read_webp_image(input_file);
    } else if(is_png_image(input_file)) {
        picture_pointer = read_png_image(input_file);
    } else {
        loge("Cannot determine input file format.");
        exit(EXIT_FAILURE);
    }
    free(input_file);
    set_width_and_height(picture_pointer);
    drawable_config *config = malloc(sizeof(drawable_config));
    config->quality = 100.0;
    config->width = width;
    config->height = height;
    config->name = name;
    config->exclude_ldpi = exclude_ldpi;
    config->exclude_mdpi = exclude_mdpi;
    config->exclude_hdpi = exclude_hdpi;
    config->exclude_xhdpi = exclude_xhdpi;
    config->exclude_xxhdpi = exclude_xxhdpi;
    config->exclude_xxxhdpi = exclude_xxxhdpi;

    write_android_drawables(picture_pointer, output_folder, config);


    free(config);
    free(name);
    free(picture_pointer->argb_pixels);
    free(picture_pointer);
    free(output_folder);
    return EXIT_SUCCESS;
}
