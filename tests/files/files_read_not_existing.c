#include <stdio.h>
#include <stdlib.h>
#include "../../src/errors/errors.h"
#include "../../src/files/commons.h"

int main(int argc, char *argv[]) {
    init_error_queue();
    open_file("not_existing_file", "rb");
    free_error_queue();
    return EXIT_SUCCESS;
}