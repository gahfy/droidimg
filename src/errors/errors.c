#include "errors.h"
#include "../logging/logging.h"
#include "../utils/string_utils.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdio.h>

/**
 * Type which defines a queue of error.
 */
typedef struct error_queue {
    char **queue;      /**< The list of messages in the queue */
    size_t queue_size; /**< The size of the queue */
    size_t total_size; /**< The size of the queue pointer */
} error_queue;

/**
 * The queue of errors of the application.
 */
static error_queue *errors = NULL;

static void validate_initialization();
static void ensure_queue_size();
static void allocate_queue_item_for_error_message(
    char *restrict message, size_t message_size
);
static void allocate_queue_item_for_error_message(
    char *restrict message, size_t message_size
);
static void init_new_error_to_queue();

void init_error_queue() {
    errors = malloc(sizeof(error_queue));
    if(errors == NULL) {
        loge("Failed to allocate memory for errors pointer.\n");
        exit(EXIT_FAILURE);
    }
    errors->queue = malloc(sizeof(char *) * 0);
    if(errors->queue == NULL) {
        loge("Failed to allocate memory for errors queue.\n");
        exit(EXIT_FAILURE);
    }
    errors->queue_size = 0;
    errors->total_size = 0;
}

void free_error_queue() {
    if(errors == NULL) {
        loge("Freeing errors pointer which has not been initialized.\n");
        exit(EXIT_FAILURE);
    }
    for(int i=0; i<errors->total_size; i++) {
        free(errors->queue[i]);
    }
    free(errors);
}

void add_error_message_to_queue(char *restrict error_message) {
    validate_initialization();
    ensure_queue_size();
    size_t message_size = strlen(error_message);
    allocate_queue_item_for_error_message(error_message, message_size);
    copy_string(errors->queue[errors->queue_size], error_message, message_size);
    if(errors->queue[errors->queue_size][message_size-1] != '\n') {
        errors->queue[errors->queue_size][message_size] = '\n';
        errors->queue[errors->queue_size][message_size+1] = 0x00;
    }
    errors->queue_size++;
}

void remove_last_error() {
    validate_initialization();
    if(errors->queue_size == 0) {
        loge("Removing last error from an empty queue.\n");
        exit(EXIT_FAILURE);
    }
    errors->queue_size--;
}

void print_errors() {
    if(errors->queue_size == 0) {
        return;
    }
    loge(errors->queue[0]);
    for(int i=1; i<errors->queue_size; i++) {
        loge("Caused by: %s", errors->queue[i]);
    }
}

/**
 * Checks that the errors is not null. If it is, then the program will end with
 * and error message.
 */
static void validate_initialization() {
    if(errors == NULL) {
        loge(
            "Adding/Removing message to errors queue before initializing it.\n"
        );
        exit(EXIT_FAILURE);
    }
}

/**
 * Ensures that a new element can be added to the queue. This is done by adding
 * one more element if there is no one yet, or freeing the existing one.
 */
static void ensure_queue_size() {
    if(errors->queue_size == errors->total_size) {
        init_new_error_to_queue();
    } else {
        free(errors->queue[errors->queue_size]);
    }
}

/**
 * Allocate sufficient memory for the given error message in the queue of
 * errors.
 * The program will end with an error if the allocation failed.
 *
 * @param error_message The error message that will be added to the queue
 * @param message_size  The length of the error message
 */
static void allocate_queue_item_for_error_message(
    char *restrict message, size_t message_size
){
    size_t memory_to_allocate = sizeof(char) * message_size + sizeof(char);
    if(message[message_size-1] != '\n') {
        memory_to_allocate += sizeof(char);
    }
    errors->queue[errors->queue_size] = malloc(memory_to_allocate);
    if(errors->queue[errors->queue_size] == NULL) {
        print_errors();
        loge("Caused by: Failed to allocate memory for the error message.\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * Adds a new error to the queue, by incrementing the total size of the queue,
 * and reallocating the queue to add one more item.
 * The program will end with an error if the allocation failed.
 */
static void init_new_error_to_queue() {
    size_t new_size = errors->total_size + 1;
    void *result = realloc(errors->queue, sizeof(char *) * new_size);
    if(result == NULL) {
        print_errors();
        loge("Caused by: Failed to reallocate memory for the queue.\n");
        exit(EXIT_FAILURE);
    }
    errors->queue = result;
    errors->total_size++;
}