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

static char *input_file = NULL;
static char *output_folder = NULL;
static char *name = NULL;
static uint32_t width = 0;
static uint32_t height = 0;

static void usage() {
    printf("Usage\n");
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
    }
    index[0] += 2;
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
        if(index == previous_index)
            usage();
        previous_index = index;
    }
}

static void set_output_folder_value() {
    if(output_folder == NULL) {
        output_folder = "./";
    } else {
        if(   get_last_character(output_folder) != '/'
           && get_last_character(output_folder) != '\\') {
            char *char_to_add = "/";
            #ifdef _WIN32
                char_to_add = "\\";
            #endif
            add_suffix_string(output_folder, char_to_add);
        }
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
    if(width == 0 || height == 0) {
        if(width == 0 && height == 0) {
            width = picture->width / 4;
            height = picture->height / 4;
        } else if(width == 0) {
            width = picture->width * height / picture->height;
        } else if(height == 0) {
            height = picture->height * width / picture->width;
        }
    }
}

static void validate_arguments() {
    if(
        input_file == NULL ||
        width == -1 ||
        height == -1
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
