#include <stdio.h>
#include <stdlib.h>
#include "../../src/errors/errors.h"

int main(int argc, char *argv[]) {
    free_error_queue();
    return EXIT_SUCCESS;
}