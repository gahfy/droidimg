#ifndef FILES_READER_H
#define FILES_READER_H

#include <stdio.h>
#include <stdint.h>

/**
 * Type containing the data of the file.
 */
typedef struct file_data {
    uint8_t *data_pointer; /**< The pointer to the list of bytes of the file*/
    size_t size;           /**< The data size (size in bytes of the file)*/
} file_data;

/**
 * Initializes the file data structure and returns a pointer to it.
 * The program will end with an error if allocation failed.
 *
 * @returns the pointer to the file data structure that has been initialized
 */
file_data *init_file_data();

/**
 * Free the memory used by the given file data stricture.
 *
 * @param file_data_pointer the pointer to the file data to desallocate
 */
void free_file_data(file_data *restrict file_data_pointer);

/**
 * Populates the content of the given file into the given file data.
 *
 * @param file_data_pointer the pointer to the file data to populate
 * @param file_path         the path to the file to read
 */
void read_file(
    file_data *restrict file_data_pointer,
    const char *restrict file_path
);

#endif  /* FILES_READER_H */
