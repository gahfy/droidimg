#include <stdio.h>
#include <stdlib.h>
#include "../../src/files/commons.h"

int main(int argc, char *argv[]) {
    open_file("files/no_permission_file", "rb");
    return EXIT_SUCCESS;
}