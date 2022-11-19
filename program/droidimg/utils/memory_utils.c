#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "constants.h"

void *allocate(size_t memory_size) {
    void *result = malloc(memory_size);
    if(result == NULL) {
        fprintf(stderr, ERROR_MESSAGE_MEMORY);
        exit(ERROR_CODE_MEMORY);
    }
    return result;
}

FILE *open_file(char *file_path, char *mode) {
    FILE *result = fopen(file_path, mode);
    if(result == NULL) {
        fprintf(
            stderr,
            "Failed to read %s: %s.\n",
            file_path,
            strerror(errno)
        );
        exit(ERROR_CODE_FILE);
    }
    return result;
}

void create_directory_if_not_exists(char *directory) {
    struct stat st = {0};
    if (stat(directory, &st) == -1) {
        int result = -1;
        #ifdef _WIN32
            result = mkdir(directory);
        #else
            result = mkdir(directory, 0777);
        #endif
        if(result != 0) {
            fprintf(
                stderr,
                "Failed to create directory %s: %s.\n",
                directory,
                strerror(errno)
            );
            exit(ERROR_CODE_FILE);
        }
    }
}
