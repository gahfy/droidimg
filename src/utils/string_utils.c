#include <stdio.h>
#include <stdlib.h>
#include "string_utils.h"
#include "constants.h"

static uint32_t add_digit_at_end(uint32_t number, char digit) {
    if(digit < 0x30 || digit > 0x39) {
        fprintf(stderr, ERROR_MESSAGE_NUMBER_FORMAT);
        exit(ERROR_CODE_NUMBER_FORMAT);
    }
    if(number > UINT32_MAX / 10 || number > UINT32_MAX - digit + 0x30) {
        fprintf(stderr, ERROR_MESSAGE_NUMBER_LIMIT);
        exit(ERROR_CODE_NUMBER_LIMIT);
    }
    return number * 10 - 0x30 + digit;
}

static void copy_until_end(char *to_copy, char *receiver, int *receiver_index) {
    int index = 0;
    while(to_copy[index] != 0x00) {
        receiver[receiver_index[0] + index] = to_copy[index];
        index++;
    }
    receiver_index[0]+=index;
}


void copy_partial_string(
    char *to_copy,
    int start_index,
    int length,
    char *receiver
) {
    for(int i=start_index; i<start_index+length; i++) {
        receiver[i - start_index] = to_copy[i];
    }
    receiver[length] = 0x00;
}

uint32_t string_to_uint32(char *string) {
    int index = 0;
    uint32_t result = 0;
    if(string[index] == 0x30 && string[index+1] != 0x00) {
        fprintf(stderr, "Leading zeros are not allowed. Provided value: %s", string);
        return -1;
    }
    while(string[index] != 0x00) {
        result = add_digit_at_end(result, string[index]);
        index++;
    }
    return result;
}

void copy_string(char *to_copy, char *receiver) {
    int index = 0;
    copy_until_end(to_copy, receiver, &index);
    receiver[index] = 0x00;
}

char get_last_character(char *char_pointer) {
    return char_pointer[get_string_length(char_pointer) - 1];
}

void add_suffix_string(char *string, char *suffix) {
    int string_length = get_string_length(string);
    int suffix_length = get_string_length(suffix);
    for(int i=0; i<suffix_length; i++) {
        string[string_length+i] = suffix[i];
    }
    string[string_length+suffix_length] = 0x00;
}

int get_last_index(char *string, char character, int start_index) {
    int result = -1;
    int index = start_index;
    while(string[index] != 0x00) {
        if(string[index] == character) {
            result = index;
        }
        index++;
    }
    return result;
}

int get_string_length(char *string) {
    int index = 0;
    while(string[index] != 0x00) {
        index++;
    }
    return index;
}
