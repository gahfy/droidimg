#include "reader.h"
#include "commons.h"
#include "../errors/errors.h"
#include "../logging/logging.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * The size of the buffer used to read the content of a file
 */
static const size_t BUFFER_SIZE = 4096;
/**
 * The length of the error message (except the file path).
 * Failed to read data of %s\n (24 characters + 1 for the final NULL char)
 */
static const size_t READ_ERROR_MESSAGE_LENGTH = 25;

static void add_read_file_error_message(const char *restrict file_path);
static void reset_file_data(file_data *restrict file_data_pointer);
static uint8_t *init_buffer();
static void set_file_content_in_file_data(
    FILE *restrict file_pointer,
    file_data *restrict file_data_pointer,
    uint8_t *restrict buffer
);
static void validate_file_data(file_data *restrict file_data_pointer);
static void reset_file_null_data(file_data *restrict file_data_pointer);
static void reset_file_not_null_data(file_data *restrict file_data_pointer);
static void add_buffer_to_file_data(
    uint8_t *restrict buffer, int size, file_data *restrict file_data_pointer
);
static void realloc_file_data_pointer(
    file_data *restrict file_data_pointer, int size_read
);

file_data *init_file_data() {
    file_data *result = malloc(sizeof(file_data));
    if(result == NULL) {
        add_error_message_to_queue(
            "Failed to allocate sufficient memory to initialize file data.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
    result->size = 0;
    result->data_pointer = NULL;
    return result;
}

void free_file_data(file_data *restrict file_data_pointer) {
    if(file_data_pointer->data_pointer != NULL) {
        free(file_data_pointer->data_pointer);
    }
    if(file_data_pointer != NULL) {
        free(file_data_pointer);
    }
}

void read_file(
    file_data *restrict file_data_pointer,
    const char *restrict file_path
) {
    add_read_file_error_message(file_path);
    reset_file_data(file_data_pointer);

    FILE *file_pointer = open_file(file_path, "rb");
    uint8_t *buffer = init_buffer();
    set_file_content_in_file_data(file_pointer, file_data_pointer, buffer);
    free(buffer);
    fclose(file_pointer);
    remove_last_error();
}

/**
 * Adds an error message for reading file content.
 *
 * @param file_path The path of the file that is being read.
 */
static void add_read_file_error_message(const char *restrict file_path) {
    char *error_message = malloc(READ_ERROR_MESSAGE_LENGTH + strlen(file_path));
    if(error_message == NULL)
        add_error_message_to_queue("Failed to read data of file.\n");
    else {
        sprintf(error_message, "Failed to read data of %s\n", file_path);
        add_error_message_to_queue(error_message);
        free(error_message);
    }
}

/**
 * Resets the given file data. If the file data already has content, then a
 * warning message will be displayed to the user, indicating that the data of
 * the given pointer will be erased.
 *
 * @param file_data_pointer the pointer to the file data to reset.
 */
static void reset_file_data(file_data *restrict file_data_pointer) {
    validate_file_data(file_data_pointer);
    if(file_data_pointer->size != 0) {
        logw("Given file_data structure is not empty. ");
        logw("Its content will be erased.\n");
        file_data_pointer->size = 0;
    }
    if(file_data_pointer->data_pointer == NULL) {
        reset_file_null_data(file_data_pointer);
    } else {
        reset_file_not_null_data(file_data_pointer);
    }
}

/**
 * Initializes a buffer and returns its value. The program will end if
 * allocation of the file data failed.
 *
 * @returns the buffer that has been initialized.
 */
static uint8_t *init_buffer() {
    uint8_t *result = malloc(sizeof(uint8_t) * BUFFER_SIZE);
    if(result == NULL) {
        add_error_message_to_queue(
            "Failed to allocate sufficient memory for the file reader buffer.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
    return result;
}

/**
 * Sets the content of the given file to the given file data.
 *
 * @param file_pointer      The pointer to the file from which to read the
 *                          content
 * @param file_data_pointer The pointer to the file data to populate
 * @param buffer            The pointer to the buffer to use to read file data
 */
static void set_file_content_in_file_data(
    FILE *restrict file_pointer,
    file_data *restrict file_data_pointer,
    uint8_t *restrict buffer
) {
    while(!feof(file_pointer)) {
        int size_read = fread(
            buffer, sizeof(uint8_t), BUFFER_SIZE, file_pointer
        );
        add_buffer_to_file_data(buffer, size_read, file_data_pointer);
    }
}

/**
 * Checks that the given file data is not NULL, and exits the program with an
 * error if it is the case.
 *
 * @param file_data_pointer The pointer to the file data to validate
 */
static void validate_file_data(file_data *restrict file_data_pointer) {
    if(file_data_pointer == NULL) {
        add_error_message_to_queue(
            "The file_data structure has not been initialized.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
}

/**
 * Resets the given file data. Is is assumed that data_pointer of the given file
 * data is NULL. You should check it before calling this.
 * The program will end with an error message if the allocation failed.
 *
 * @param file_data_pointer the pointer to the file data to reset
 */
static void reset_file_null_data(file_data *restrict file_data_pointer) {
    file_data_pointer->data_pointer = malloc(sizeof(uint8_t) * 0);
    if(file_data_pointer->data_pointer == NULL) {
        add_error_message_to_queue(
            "Failed to allocate sufficient memory to read data.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
}

/**
 * Resets the given file data. It is assumed that data_pointer of the given file
 * data is not NULL. You should check it before calling this.
 * The program will end with an error message if allocation failed.
 *
 * @param file_data_pointer the pointer to the file data to reset
 */
static void reset_file_not_null_data(file_data *restrict file_data_pointer) {
    free(file_data_pointer->data_pointer);
    file_data_pointer->data_pointer = realloc(
        file_data_pointer->data_pointer,
        sizeof(uint8_t) * 0
    );
    if(file_data_pointer->data_pointer == NULL) {
        add_error_message_to_queue(
            "Failed to allocate sufficient memory to read data.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
}

/**
 * Adds the content of the given buffer to the given file data.
 * 
 * @param buffer            The buffer containing the data to add to the given
 *                          file data
 * @param size              The size of the data in the buffer
 * @param file_data_pointer The pointer to the file data to which to add the
 *                          content of the given buffer
 */
static void add_buffer_to_file_data(
    uint8_t *restrict buffer, int size, file_data *restrict file_data_pointer
) {
    if(size == 0)
        return;
    realloc_file_data_pointer(file_data_pointer, size);
    memcpy(
        &file_data_pointer->data_pointer[file_data_pointer->size],
        buffer,
        size
    );
    file_data_pointer->size += size;
}

/**
 * Reallocate the data of the given file data by adding the given size to
 * the memory already allocated.
 *
 * @param file_data_pointer the pointer to the file data containing the data to
 *                          expand
 * @param size_to_add       The size of the memory to add to the data of the
 *                          given file data
 */
static void realloc_file_data_pointer(
    file_data *restrict file_data_pointer, int size_to_add
) {
    file_data_pointer->data_pointer = realloc(
        file_data_pointer->data_pointer,
        file_data_pointer->size + size_to_add * sizeof(uint8_t)
    );
    if(file_data_pointer->data_pointer == NULL) {
        add_error_message_to_queue(
            "Failed to reallocate sufficient memory to read data.\n"
        );
        print_errors();
        exit(EXIT_FAILURE);
    }
}