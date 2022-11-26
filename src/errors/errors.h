#ifndef ERRORS_ERRORS_H
#define ERRORS_ERRORS_H

/**
 * Initialize the queue of errors. This should be called before any other
 * function related to errors.
 */
void init_error_queue();

/**
 * Desallocate the memory used by the error queue. This should be called when
 * errors are not needed anymore.
 */
void free_error_queue();

/**
 * Adds the given error message to the queue of error messages.
 *
 * @param error_message The error message to add to the queue
 */
void add_error_message_to_queue(char *error_message);

/**
 * Removes the last error message from the queue of error messages.
 */
void remove_last_error();

/**
 * Prints all the errors in the queue.
 */
void print_errors();

#endif  /* ERRORS_ERRORS_H */