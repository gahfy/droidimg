#ifndef FILES_COMMONS_H
#define FILES_COMMONS_H

#include <stdio.h>
#include <stdint.h>

/**
 * Opens a file and returns the pointer to it or exits the program with an
 * error message if opening the file failed.
 *
 * @param file_path The path of the file to open
 * @param mode      The mode to determine how the file will be treated
 *
 * @return a pointer to the file that has been opened.
 */
FILE *open_file(const char *restrict file_path, const char *restrict mode);

#endif  /* FILES_COMMONS_H */