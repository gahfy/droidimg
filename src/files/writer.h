#ifndef FILES_WRITER_H
#define FILES_WRITER_H

#include <stdio.h>
#include <stdint.h>

/**
 * Creates the directory at the given path if it does not exists, otherwise do
 * nothing.
 *
 * @param directory the directory to create
 */
void create_directory_if_not_exists(char *restrict directory);

#endif  /* FILES_WRITER_H */