#include "reader.h"
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#ifndef _WIN32
#include <pwd.h>
#endif
#include "../logging/logging.h"
#include "../files/commons.h"

typedef struct parsing_buffer{

    // 0 : wait for destination
    // 1 : parsing destination name
    // 2 : end parsing destination name (expecting :)
    // 3 : found : (expecting folder)
    // 4 : parsing folder
    int status;
    int prefix_index;
    int destination_name_index;
    bool still_good;
    bool finished;
    char *dir;
    char *destination_name;
} parsing_buffer;

static const char* destination_prefix = "destination.";

static void handle_line_break(parsing_buffer *restrict buffer) {
    if(buffer->destination_name_index != 0) {
        buffer->finished = true;
    } else {
        buffer->destination_name_index = 0;
        buffer->prefix_index = 0;
        buffer->still_good = true;
        buffer->status = 0;
    }
}

static void handle_first_spaces(char current, parsing_buffer *restrict buffer) {
    if(current != '\t' && current != ' ') {
        buffer->status = 1;
    }
}

static void handle_after_destination(parsing_buffer *restrict buffer) {
    if(buffer->destination_name[buffer->prefix_index - 11] == 0x00)
        buffer->status = 2;
    buffer->prefix_index++;
}

static void handle_prefix(char current, parsing_buffer *restrict buffer) {
    if(
        buffer->prefix_index < 12 &&
        current == destination_prefix[buffer->prefix_index]
    )
        buffer->prefix_index++;
    else if(
        buffer->prefix_index >= 12 &&
        current == buffer->destination_name[buffer->prefix_index - 12]
    )
        handle_after_destination(buffer);
    else
        buffer->still_good = false;
}

static void handle_before_colon(char current, parsing_buffer *restrict buffer) {
    // If it's a tab, a space or :
    if(current != 0x09 && current != 0x20)
        buffer->status = 3;
}

static void handle_after_colon(char current, parsing_buffer *restrict buffer) {
    if(current != '\t' && current != ' ')
        buffer->status = 4;
}

static void handle_dir_name(char current, parsing_buffer *restrict buffer) {
    if(buffer->destination_name_index < 4096) {
        buffer->dir[buffer->destination_name_index] = current;
        buffer->destination_name_index++;
    }
}

static void handle_good_status(char current, parsing_buffer *restrict buffer) {
    if(buffer->status == 0)
        handle_first_spaces(current, buffer);
    if(buffer->status == 1)
        handle_prefix(current, buffer);
    else if(buffer->status == 2)
        handle_before_colon(current, buffer);
    else if(buffer->status == 3)
        handle_after_colon(current, buffer);
    if(buffer->status == 4) handle_dir_name(current, buffer);
}

static void parse_current_char(char current, parsing_buffer *restrict buffer) {
    if(current == '\r' || current == '\n')
        handle_line_break(buffer);
    else if(buffer->still_good)
        handle_good_status(current, buffer);
}

static char *get_unix_config_file_path() {
    struct passwd *pw = getpwuid(getuid());
    size_t pointer_size = sizeof(char) * (strlen(pw->pw_dir) + 18);
    char *user_config_file_path = malloc(pointer_size);
    if(user_config_file_path == NULL) {
        loge("Failed to allocate memory for destination config file path.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(user_config_file_path, "%s/.droidimg.config", pw->pw_dir);
    return user_config_file_path;
}

static char *get_windows_config_file_path() {
    size_t pointer_size = sizeof(char) * (strlen(getenv("USERPROFILE")) + 18);
    char *user_config_file_path = malloc(pointer_size);
    if(user_config_file_path == NULL) {
        loge("Failed to allocate memory for destination config file path.\n");
        exit(EXIT_FAILURE);
    }
    sprintf(
        user_config_file_path,
        "%s/.droidimg.config",
        getenv("USERPROFILE")
    );
    return user_config_file_path;
}

static char *get_config_file_path() {
    #ifdef _WIN32
        return get_windows_config_file_path();
    #else
       return get_unix_config_file_path();
    #endif
}

static void validate_buffer(parsing_buffer *restrict buffer) {
    if(buffer == NULL) {
        loge("Failed to allocate memory for the configuration buffer.\n");
        exit(EXIT_FAILURE);
    }
}

static parsing_buffer *init_buffer(
    char *restrict destination_name,
    char *restrict dir
) {
    parsing_buffer *buffer = malloc(sizeof(parsing_buffer));
    validate_buffer(buffer);
    buffer->status = 0;
    buffer->prefix_index = 0;
    buffer->destination_name_index = 0;
    buffer->still_good = true;
    buffer->finished = false;
    buffer->dir = dir;
    buffer->destination_name = destination_name;
    return buffer;
}

static void validate_char_buffer(char *restrict buffer) {
    if(buffer == NULL) {
        loge("Failed to allocate memory for destination buffer.\n");
        exit(EXIT_FAILURE);
    }
}

static char *init_result() {
    char *result = malloc(sizeof(char) * 4097);
    if(result == NULL) {
        loge("Failed to allocate memory for the result directory.\n");
        exit(EXIT_FAILURE);
    }
    result[0] = 0x00;
    return result;
}

char *get_config_folder(char *destination_name) {
    char *result = init_result();
    char *user_config_file_path = get_config_file_path();
    FILE *file_pointer = open_file(user_config_file_path, "rt");
    parsing_buffer *buffer = init_buffer(destination_name, result);
    char *current = malloc(sizeof(char) * 2);
    validate_char_buffer(current);
    while(fgets(current, sizeof(char) * 2, file_pointer) && !buffer->finished)
        parse_current_char(current[0], buffer);
    free(current);
    if(result != NULL && result[buffer->destination_name_index-1] != 0x00)
        result[buffer->destination_name_index] = 0x00;
    fclose(file_pointer);
    return result;
}