#include <stdio.h>
#include <stdlib.h>
#include "../../src/errors/errors.h"
#include "../../src/files/commons.h"
#include "../../src/files/reader.h"

int main(int argc, char *argv[]) {
    init_error_queue();
    file_data *file_data_pointer = init_file_data();
    read_file(file_data_pointer, "./files/empty_file");
    if(file_data_pointer->size != 0) {
        exit(EXIT_FAILURE);
    }
    free_file_data(file_data_pointer);
    free_error_queue();
    return EXIT_SUCCESS;
}