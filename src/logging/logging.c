#include "logging.h"
#include <stdio.h>
#include <stdarg.h>

/** The current level of logs in use in the application */
static log_level current_level = Warning;

static int print_log(log_level level, char *format, va_list arguments);

int logv(char *format, ...) {
    va_list args;              // Use argument a somehow
    va_start(args, format);
    return print_log(Verbose, format, args);
    va_end(args);
}

int logd(char *format, ...) {
    va_list args;              // Use argument a somehow
    va_start(args, format);
    return print_log(Debug, format, args);
    va_end(args);
}

int logi(char *format, ...) {
    va_list args;              // Use argument a somehow
    va_start(args, format);
    return print_log(Info, format, args);
    va_end(args);
}

int logw(char *format, ...) {
    va_list args;              // Use argument a somehow
    va_start(args, format);
    return print_log(Warning, format, args);
    va_end(args);
}

int loge(char *format, ...) {
    va_list args;              // Use argument a somehow
    va_start(args, format);
    return print_log(Error, format, args);
    va_end(args);
}

void set_log_level(log_level new_level) {
    current_level = new_level;
}

/**
 * Prints a message if the current log level includes the given level, and
 * returns the number of character printed (0 if the current level does not
 * include the given level).
 *
 * @param level     The level of the message to print
 * @param format    The format of the message to print
 * @param arguments The arguments used to print the message
 *
 * @return the number of characters that have been printed
 */
int print_log(log_level level, char *format, va_list arguments) {
    if(level >= current_level) {
        return vfprintf(level >= Error ? stderr : stdout, format, arguments);
    }
    return 0;
}
