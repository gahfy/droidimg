#include <stdio.h>
#include <stdlib.h>
#include "../../src/files/commons.h"

int main(int argc, char *argv[]) {
    FILE *test = open_file("files/no_permission_file", "wb");
    return EXIT_SUCCESS;
}