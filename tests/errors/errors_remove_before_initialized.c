#include <stdio.h>
#include <stdlib.h>
#include "../../src/errors/errors.h"

int main(int argc, char *argv[]) {
    remove_last_error();
    return EXIT_SUCCESS;
}