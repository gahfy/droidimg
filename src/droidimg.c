#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <png.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <webp/encode.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils/png_utils.h"
#include "utils/picture_utils.h"
#include "utils/memory_utils.h"
#include "utils/string_utils.h"
#include "utils/webp_utils.h"
#include "utils/constants.h"
#include "utils/android_utils.h"
#include "utils/config_utils.h"
#include "errors/errors.h"

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

static void parse_input_file(int argc, char *argv[], int *index) {
    input_file = allocate(sizeof(char) * (strlen(argv[index[0]]) + 1));
    copy_string(input_file, argv[index[0]], strlen(argv[index[0]]));
    index[0]++;
}

static void parse_output_folder(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--output") * strcmp(argv[index[0]], "-o") == 0) {
        // +2 because we may add a final / or \ at the end of the name
        output_folder = allocate(sizeof(char) * (strlen(argv[index[0]+1]) + 2));
        copy_string(output_folder, argv[index[0]+1], strlen(argv[index[0]+1]));
        index[0] += 2;
    }
}

static void parse_name(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--name") * strcmp(argv[index[0]], "-n") == 0) {
        name = allocate(sizeof(char) * (strlen(argv[index[0]+1]) + 1));
        copy_string(name, argv[index[0]+1], strlen(argv[index[0]+1]));
        index[0] += 2;
    }
}

static void parse_width(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--width") * strcmp(argv[index[0]], "-w") == 0) {
        width = string_to_uint32(argv[index[0]+1]);
        index[0] += 2;
    }
}

static void parse_height(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--height") * strcmp(argv[index[0]], "-h") == 0) {
        height = string_to_uint32(argv[index[0]+1]);
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
        destination = allocate(sizeof(char) * (strlen(argv[index[0]+1]) + 1));
        copy_string(destination, argv[index[0]+1], strlen(argv[index[0]+1]));
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
                exit(ERROR_CODE_ARGUMENTS);
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
            exit(ERROR_CODE_ARGUMENTS);
        }
        previous_index = index;
    }
}

static void set_output_folder_from_destination() {
    if(destination == NULL) {
        output_folder = allocate(3 * sizeof(char));
        sprintf(output_folder, "./");
    } else {
        // Allocate the max length of a folder
        output_folder = allocate(sizeof(char) * 4097);
        set_destination(destination, output_folder);
        if(output_folder[0] == 0x00) {
            free(output_folder);
            output_folder = allocate(3 * sizeof(char));
            sprintf(output_folder, "./");
        }
        free(destination);
    }
}

static void set_output_folder_value() {
    if(output_folder == NULL) {
        set_output_folder_from_destination();
    }
    if(   get_last_character(output_folder) != '/'
        && get_last_character(output_folder) != '\\') {
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
        int start_index = get_last_index(input_file, '/', 0);
#ifdef _WIN32
        if(start_index == -1)
            get_last_index(input_file, '\\', 0);
#endif
        start_index = start_index == -1 ? 0 : start_index + 1;
        int end_index = get_last_index(input_file, '.', start_index);
        if (end_index == -1)
            end_index = strlen(input_file) - 1;

        int length = end_index - start_index;
        name = allocate(sizeof(char) * (length + 1));
        copy_string(name, &input_file[start_index], length);
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
                fprintf(stderr, ERROR_MESSAGE_ARGUMENTS);
                exit(ERROR_CODE_ARGUMENTS);
            }
            width = (uint32_t) result;
        } else if(height == UINT32_MAX) {
            uint64_t result = ((uint64_t) picture->height) * (uint64_t) width;
            result /= (uint64_t) picture->width;
            if(result > UINT32_MAX / 4) {
                fprintf(stderr, ERROR_MESSAGE_ARGUMENTS);
                exit(ERROR_CODE_ARGUMENTS);
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
        fprintf(stderr, ERROR_MESSAGE_ARGUMENTS);
        exit(ERROR_CODE_ARGUMENTS);
    }
}

int main(int argc, char *argv[]) {
    init_error_queue();
    parse_arguments(argc, argv);
    validate_arguments();
    set_output_folder_value();
    set_name_from_input_file();
    picture *picture_pointer = get_picture_from_png_file(input_file);
    free(input_file);
    set_width_and_height(picture_pointer);
    write_android_files(
        picture_pointer,
        width,
        height,
        output_folder,
        name,
        exclude_ldpi,
        exclude_mdpi,
        exclude_hdpi,
        exclude_xhdpi,
        exclude_xxhdpi,
        exclude_xxxhdpi
    );
    free(name);
    free(picture_pointer->argb_pixels);
    free(picture_pointer);
    free(output_folder);
    free_error_queue();
    return EXIT_SUCCESS;
}
