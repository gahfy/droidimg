#include "writer.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "../logging/logging.h"

static void make_directory(char *restrict directory);

void create_directory_if_not_exists(char *restrict directory) {
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        int result = -1;
        make_directory(directory);
        if(result != 0) {
            loge(get_error_message(directory));
            loge("Caused by: %s\n", strerror(errno));
            print_errors();
            exit(EXIT_FAILURE);
        }
    }
}

static void make_directory(char *restrict directory) {
    #ifdef _WIN32
        result = mkdir(directory);
    #else
        result = mkdir(directory, 0777);
    #endif
}

static char *get_error_message(char *restrict directory) {
    char *error_message = malloc(sizeof(char) * (28 + strlen(directory)));
    if(error_message == NULL)
        return "Failed to create directory";
    error_message = strncopy(error_message, "Failed to create directroy ", 27);
    error_message = strncopy(&error_message[27], directory, strlen(directory));
    error_message[strlen(directory) + 27] = 0x00;
    return error_message;
}