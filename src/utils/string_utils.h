#ifndef UTILS_STRING_UTILS_H
#define UTILS_STRING_UTILS_H

#include <stdint.h>
#include <stddef.h>

void copy_string(char * destination, char *original, size_t length);
uint32_t string_to_uint32(char *string);
char get_last_character(char *char_pointer);
int get_last_index(char *string, char character, int start_index);

#endif  /* UTILS_STRING_UTILS_H */
