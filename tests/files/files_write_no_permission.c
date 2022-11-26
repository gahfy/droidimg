#include <stdio.h>
#include <stdlib.h>
#include "../../src/errors/errors.h"
#include "../../src/files/commons.h"

int main(int argc, char *argv[]) {
    init_error_queue();
    FILE *test = open_file("files/no_permission_file", "wb");
    free_error_queue();
    return EXIT_SUCCESS;
}