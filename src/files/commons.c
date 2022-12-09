#include "commons.h"
#include "../logging/logging.h"
#include <stdlib.h>
#include <string.h>
#include <errno.h>

static void log_error_message(
    const char *restrict file_path,
    const char *restrict mode
);
static void log_allocated_open_error_message(
    char *error_message,
    const char *restrict file_path,
    const char *restrict mode
);
static void log_generic_open_error_message(const char *restrict mode);

FILE *open_file(const char *restrict file_path, const char *restrict mode) {
    FILE *file_pointer = fopen(file_path, mode);
    if(file_pointer == NULL) {
        log_error_message(file_path, mode);
        loge("Caused by: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    return file_pointer;
}

/**
 * Logs an error message for opening a file.
 *
 * @param file_path The path of the file to open
 * @param mode      The mode passed to fopen to determine how the file will be
 *                  treated
 */
static void log_error_message(
    const char *restrict file_path,
    const char *restrict mode
) {
    char *error_message = malloc(22 + strlen(file_path));
    if(error_message == NULL) {
        log_generic_open_error_message(mode == NULL ? "" : mode);
    } else {
        log_allocated_open_error_message(
            error_message, file_path, mode == NULL ? "" : mode
        );
        free(error_message);
    }
}

/**
 * Logs a message error for opening a file containing the path to the file. The
 * error_message argument should have been allocated before. If allocation
 * failed, call add_generic_open_error_message(const char *restrict mode)
 * method instead.
 *
 * @param error_message The pointer to the error message that has been allocated
 * @param file_path     The path of the file to open
 * @param mode          The mode passed to fopen to determine how the file will
 *                      be treated
 */
static void log_allocated_open_error_message(
    char *error_message,
    const char *restrict file_path,
    const char *restrict mode
) {
    if(strlen(mode) < 1 || (mode[0] != 'a' && mode[0] != 'r' && mode[0] != 'w'))
        sprintf(error_message, "Failed to open %s\n", file_path);
    else if(mode[0] == 'a')
        sprintf(error_message, "Failed to append to %s\n", file_path);
    else if(mode[0] == 'r')
        sprintf(error_message, "Failed to read %s\n", file_path);
    else
        sprintf(error_message, "Failed to write %s\n", file_path);
    loge(error_message);
}

/**
 * Adds a generic error message for opening a file. This method should be called
 * if the allocation for an error message containing the file name failed to be
 * allocated.
 *
 * @param mode The mode passed to fopen to determine how the file will be
 *             treated
 */
static void log_generic_open_error_message(const char *restrict mode) {
    if(strlen(mode) < 1 || (mode[0] != 'a' && mode[0] != 'r' && mode[0] != 'w'))
        loge("Failed to write file.\n");
    else if(mode[0] == 'a')
        loge("Failed to append to file.\n");
    else if(mode[0] == 'r')
        loge("Failed to read file.\n");
    else
        loge("Failed to write file.\n");
}