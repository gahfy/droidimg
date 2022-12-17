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
#include "config/reader.h"
#include "logging/logging.h"
#include "android/drawables.h"

typedef struct arguments{
    char *input_file;
    char *output_folder;
    char *destination;
    char *name;
    uint32_t width;
    uint32_t height;
    bool exclude_ldpi;
    bool exclude_mdpi;
    bool exclude_hdpi;
    bool exclude_xhdpi;
    bool exclude_xxhdpi;
    bool exclude_xxxhdpi;
} arguments;

static char *input_file = NULL;
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

static void validate_output_folder(arguments *args) {
    if(args->output_folder == NULL) {
        loge("Failed to allocate memory for output folder argument.\n");
        exit(EXIT_FAILURE);
    }
}

static void parse_input_file(
    int argc, char *argv[], int *index, int previous_index
) {
    if(index[0] == previous_index && input_file == NULL) {
        input_file = malloc(sizeof(char) * (strlen(argv[index[0]]) + 1));
        if(input_file == NULL) {
            loge("Failed to allocate memory for input file argument.\n");
            exit(EXIT_FAILURE);
        }
        memcpy(input_file, argv[index[0]], strlen(argv[index[0]])+1);
        index[0]++;
    }
}

static void parse_output_folder(
    int argc, char *argv[], int *index, arguments *args
) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--output") * strcmp(argv[index[0]], "-o") == 0) {
        size_t output_folder_length = strlen(argv[index[0]+1]) + 1;
        args->output_folder = malloc(sizeof(char) * (output_folder_length + 1));
        validate_output_folder(args);
        memcpy(args->output_folder, argv[index[0]+1], output_folder_length);
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

static void validate_destination(char *restrict destination) {
    if(destination == NULL) {
        loge("Failed to allocate memory for destination argument.\n");
        exit(EXIT_FAILURE);
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
        validate_destination(destination);
        memcpy(destination, argv[index[0]+1], strlen(argv[index[0]+1])+1);
        index[0] += 2;
    }
}

static bool is_l(char *argv[], int index, int *current) {
    if(
            argv[index+1][current[0]] == 'l'
        && (   argv[index+1][current[0]+1] == ','
            || argv[index+1][current[0]+1] == 0x00
            )
    ) {
        current[0] += (argv[index+1][current[0]+1] == 0x00) ? 1 : 2;
        return true;
    }
    return false;
}

static bool is_m(char *argv[], int index, int *current) {
    if(
            argv[index+1][current[0]] == 'm'
        && (   argv[index+1][current[0]+1] == ','
            || argv[index+1][current[0]+1] == 0x00
            )
    ) {
        current[0] += (argv[index+1][current[0]+1] == 0x00) ? 1 : 2;
        return true;
    }
    return false;
}

static bool is_h(char *argv[], int index, int *current) {
    if(
            argv[index+1][current[0]] == 'h'
        && (   argv[index+1][current[0]+1] == ','
            || argv[index+1][current[0]+1] == 0x00
            )
    ) {
        current[0] += (argv[index+1][current[0]+1] == 0x00) ? 1 : 2;
        return true;
    }
    return false;
}

static bool is_x(char *argv[], int index, int *current) {
    if(
            argv[index+1][current[0]] == 'x'
        && (   argv[index+1][current[0]+1] == ','
            || argv[index+1][current[0]+1] == 0x00
            )
    ) {
        current[0] += (argv[index+1][current[0]+1] == 0x00) ? 1 : 2;
        return true;
    }
    return false;
}

static bool is_xx(char *argv[], int index, int *current) {
    if(
            argv[index+1][current[0]] == 'x'
        &&  argv[index+1][current[0]+1] == 'x'
        && (   argv[index+1][current[0]+2] == ','
            || argv[index+1][current[0]+2] == 0x00
            )
    ) {
        current[0] += (argv[index+1][current[0]+2] == 0x00) ? 1 : 2;
        return true;
    }
    return false;
}

static bool is_xxx(char *argv[], int index, int *current) {
    if(
            argv[index+1][current[0]] == 'x'
        &&  argv[index+1][current[0]+1] == 'x'
        &&  argv[index+1][current[0]+2] == 'x'
        && (   argv[index+1][current[0]+3] == ','
            || argv[index+1][current[0]+3] == 0x00
            )
    ) {
        current[0] += (argv[index+1][current[0]+3] == 0x00) ? 1 : 2;
        return true;
    }
    return false;
}

static void loop_exclusions(char *argv[], int *index, int *current) {
    exclude_ldpi = exclude_ldpi | is_l(argv, *index, current);
    exclude_mdpi = exclude_mdpi | is_m(argv, *index, current);
    exclude_hdpi = exclude_hdpi | is_h(argv, *index, current);
    exclude_xhdpi = exclude_xhdpi | is_x(argv, *index, current);
    exclude_xxhdpi = exclude_xxhdpi | is_xx(argv, *index, current);
    exclude_xxxhdpi = exclude_xxxhdpi | is_xxx(argv, *index, current);
}

static void parse_exclude(int argc, char *argv[], int *index) {
    if(index[0] >= argc-1)
        return;
    if(strcmp(argv[index[0]], "--exclude") * strcmp(argv[index[0]], "-e") == 0) {
        int current = 0;
        int previous_current = 0;
        while(argv[index[0]+1][current] != 0x00) {
            loop_exclusions(argv, index, &current);
            if(current == previous_current) {
                usage();
                exit(EXIT_FAILURE);
            }
            previous_current = current;
        }
        index[0] += 2;
    }
}

static void handle_error(int index, int previous_index) {
    if(index == previous_index) {
        usage();
        exit(EXIT_FAILURE);
    }
}

static void loop_arguments(
    int *index, int *previous_index, int argc, char *argv[], arguments *args
) {
    parse_output_folder(argc, argv, index, args);
    parse_name(argc, argv, index);
    parse_width(argc, argv, index);
    parse_height(argc, argv, index);
    parse_destination(argc, argv, index);
    parse_exclude(argc, argv, index);
    parse_input_file(argc, argv, index, previous_index[0]);
    handle_error(index[0], previous_index[0]);
    previous_index[0] = index[0];
}

static void parse_arguments(int argc, char *argv[], arguments *args) {
    int index = 1;
    int previous_index = 1;
    while(index < argc) {
        loop_arguments(&index, &previous_index, argc, argv, args);
    }
}

static void set_output_folder_from_null_destination(arguments *args) {
    args->output_folder = malloc(3 * sizeof(char));
    validate_output_folder(args);
    sprintf(args->output_folder, "./");
}

static void set_output_folder_from_non_null_destination(
    char *restrict destination, arguments *args
) {
    char *config_folder = get_config_folder(destination);
    if(config_folder[0] == 0x00) {
        //free(args->output_folder);
        set_output_folder_from_null_destination(args);
    } else {
        args->output_folder = malloc(sizeof(char) * (strlen(config_folder)+1));
        validate_output_folder(args);
        memcpy(args->output_folder, config_folder, strlen(config_folder) + 1);
    }
    //free(config_folder);
}

static void set_output_folder_from_destination(arguments *args) {
    if(destination == NULL) {
        set_output_folder_from_null_destination(args);
    } else {
        set_output_folder_from_non_null_destination(destination, args);
        //free(destination);
    }
}

static char get_directory_separator() {
    #ifdef _WIN32
        return '\\';
    #else
        return '/';
    #endif
}

static void set_output_folder_value(arguments *args) {
    if(args->output_folder == NULL) {
        set_output_folder_from_destination(args);
    }
    if(   args->output_folder[strlen(args->output_folder) - 1] != '/'
        && args->output_folder[strlen(args->output_folder) - 1] != '\\') {
        char char_to_add = get_directory_separator();
        int index = strlen(args->output_folder);
        if(index == 4096) {
            index -= 1;
        }
        args->output_folder[index++] = char_to_add;
        args->output_folder[index] = 0x00;
    }
}

static void validate_file_name(char *restrict file_name) {
    if(file_name == NULL) {
        loge("Failed to allocate memory for name argument.\n");
        exit(EXIT_FAILURE);
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
        validate_file_name(name);
        memcpy(name, &input_file[start_index], length+1);
    }
}

static uint32_t get_other_dimension(
    uint64_t known_dimension,
    uint32_t picture_known_dimension,
    uint32_t picture_unknown_dimension
) {
    uint64_t result = (uint64_t) picture_unknown_dimension * known_dimension;
    result /= (uint64_t) picture_known_dimension;
    if(result > UINT32_MAX / 4) {
        loge("Wrong width and/or height passed to program.\n");
        exit(EXIT_FAILURE);
    }
    return (uint32_t) result;
}

static void set_width_and_height(picture *picture) {
    if(width == UINT32_MAX || height == UINT32_MAX) {
        if(width == UINT32_MAX && height == UINT32_MAX) {
            width = picture->width / 4;
            height = picture->height / 4;
        } else if(width == UINT32_MAX)
            width = get_other_dimension(
                (uint64_t) height, picture->height, picture->width
            );
        else if(height == UINT32_MAX)
            height = get_other_dimension(
                (uint64_t) width, picture->width, picture->height
            );
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

static arguments *init_arguments() {
    arguments *result = malloc(sizeof(arguments));
    if(result == NULL) {
        loge("Failed to allocate memory for arguments structure.\n");
        exit(EXIT_FAILURE);
    }
    return result;
}

static void free_arguments(arguments *restrict args) {
    if(args != NULL) {
        //if(args->input_file != NULL)
            //free(args->input_file);
        //if(args->output_folder != NULL)
            //free(args->output_folder);
        //if(args->destination != NULL)
            //free(args->destination);
        //if(args->name != NULL)
            //free(args->name);
        //free(args);
    }
}

int main(int argc, char *argv[]) {
    arguments *args = malloc(sizeof(arguments));
    parse_arguments(argc, argv, args);
    validate_arguments();
    set_output_folder_value(args);
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
    //free(input_file);
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

    write_android_drawables(picture_pointer, args->output_folder, config);


    //free(config);
    //free_arguments(args);
    //free_picture(picture_pointer);
    return EXIT_SUCCESS;
}
