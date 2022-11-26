#include <stdio.h>
#include <stdlib.h>
#include "../../src/errors/errors.h"

int main(int argc, char *argv[]) {
    init_error_queue();
    add_error_message_to_queue("Test error one");
    add_error_message_to_queue("Test error two");
    print_errors();
    return EXIT_SUCCESS;
}