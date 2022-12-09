#ifndef UTILS_STRING_UTILS_H
#define UTILS_STRING_UTILS_H

#include <stdint.h>
#include <stddef.h>

uint32_t string_to_uint32(char *string);
char get_last_character(char *char_pointer);
int get_last_index(char *string, char character, int start_index);

#endif  /* UTILS_STRING_UTILS_H */
