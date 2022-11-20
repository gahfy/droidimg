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

static char *input_file = NULL;
static char *output_folder = NULL;
static char *destination = NULL;
static char *name = NULL;
static uint32_t width = UINT32_MAX;
static uint32_t height = UINT32_MAX;

static void usage() {
    printf("usage: droidimg -i|--input <input_png_file>\n");
    printf("                [-o|--output <output_resource_folder>] ");
    printf("[-n|--name <file_name>]\n");
    printf("                [-w|--with <width_in_dp>] ");
    printf("[-h|--height <height_in_dp>]\n");
    printf("                [-d|--destination <destination_from_config>] ");
}

static void parse_input_file(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--input") * strcmp(argv[index[0]], "-i") == 0) {
        input_file = allocate(sizeof(char) * (get_string_length(argv[index[0]+1]) + 1));
        copy_string(argv[index[0]+1], input_file);
        index[0] += 2;
    }
}

static void parse_output_folder(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--output") * strcmp(argv[index[0]], "-o") == 0) {
        output_folder = allocate(sizeof(char) * (get_string_length(argv[index[0]+1]) + 1));
        copy_string(argv[index[0]+1], output_folder);
        index[0] += 2;
    }
}

static void parse_name(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--name") * strcmp(argv[index[0]], "-n") == 0) {
        name = allocate(sizeof(char) * (get_string_length(argv[index[0]+1]) + 1));
        copy_string(argv[index[0]+1], name);
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
    if(strcmp(argv[index[0]], "--destination") * strcmp(argv[index[0]], "-d") == 0) {
        destination = allocate(sizeof(char) * (get_string_length(argv[index[0]+1]) + 1));
        copy_string(argv[index[0]+1], destination);
        index[0] += 2;
    }
}

static void parse_arguments(int argc, char *argv[]) {
    int index = 1;
    int previous_index = 1;
    while(index < argc) {
        parse_input_file(argc, argv, &index);
        parse_output_folder(argc, argv, &index);
        parse_name(argc, argv, &index);
        parse_width(argc, argv, &index);
        parse_height(argc, argv, &index);
        parse_destination(argc, argv, &index);
        if(index == previous_index) {
            usage();
            exit(ERROR_CODE_ARGUMENTS);
        }
        previous_index = index;
    }
}

static void set_output_folder_from_destination() {
    if(destination == NULL) {
        output_folder = "./";
    } else {
        // Allocate the max length of a folder
        output_folder = allocate(sizeof(char) * 4097);
        set_destination(destination, output_folder);
        if(output_folder[0] == 0x00) {
            output_folder = "./";
        }
    }
}

static void set_output_folder_value() {
    if(output_folder == NULL) {
        set_output_folder_from_destination();
    }
    if(   get_last_character(output_folder) != '/'
        && get_last_character(output_folder) != '\\') {
        char *char_to_add = "/";
        #ifdef _WIN32
            char_to_add = "\\";
        #endif
        add_suffix_string(output_folder, char_to_add);
    }
}

static void set_name_from_input_file() {
    int start_index = get_last_index(input_file, '/', 0);
    #ifdef _WIN32
        if(start_index == -1)
            get_last_index(input_file, '\\', 0);
    #endif
    start_index = start_index == -1 ? 0 : start_index+1;
    int end_index = get_last_index(input_file, '.', start_index);
    if(end_index == -1)
        end_index = get_string_length(input_file) - 1;

    int length = end_index - start_index;
    name = allocate(sizeof(char) * (length+1));
    copy_partial_string(input_file, start_index, length, name);
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
    parse_arguments(argc, argv);
    validate_arguments();
    set_output_folder_value();
    set_name_from_input_file();
    picture *picture_pointer = get_picture_from_png_file(input_file);
    free(input_file);
    set_width_and_height(picture_pointer);
    write_android_files(picture_pointer, width, height, output_folder, name);
    free(name);
    free(picture_pointer->argb_pixels);
    free(picture_pointer);
    return EXIT_SUCCESS;
}
