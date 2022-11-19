#ifndef UTILS_STRING_UTILS_H
#define UTILS_STRING_UTILS_H

uint32_t string_to_uint32(char *string);
void copy_string(char *to_copy, char *receiver);
char get_last_character(char *char_pointer);
int get_last_index(char *string, char character, int start_index);
int get_string_length(char *string);
void copy_partial_string(
    char *to_copy,
    int start_index,
    int length,
    char *receiver
);
void add_suffix_string(char *string, char *suffix);

#endif  /* UTILS_STRING_UTILS_H */
