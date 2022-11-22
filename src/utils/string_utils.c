#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stddef.h>
#include "string_utils.h"
#include "constants.h"

uint32_t string_to_uint32(char *string) {
    char *end;
    unsigned long result = strtoul(string, &end, 10);
    if(end[0] != 0x00 || result > UINT32_MAX) {
        return UINT32_MAX;
    }
    return (uint32_t) result;
}

char get_last_character(char *char_pointer) {
    return char_pointer[strlen(char_pointer) - 1];
}

void copy_string(char *destination, char *original, size_t length) {
    strncpy(destination, original, length);
    // Security for Windows
    destination[length] = 0x00;
}

int get_last_index(char *string, char character, int start_index) {
    char *last_occurrence = strrchr(string, character);
    if(last_occurrence == NULL) {
        return -1;
    }
    ptrdiff_t bytes = (last_occurrence - string);
    if(bytes < start_index) {
        return -1;
    }
    return (int) bytes;
}
