#include <stdio.h>
#include <stdlib.h>
#include "../../src/errors/errors.h"

int main(int argc, char *argv[]) {
    add_error_message_to_queue("Test error");
    return EXIT_SUCCESS;
}