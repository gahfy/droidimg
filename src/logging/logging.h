#ifndef LOGGING_LOGGING_H
#define LOGGING_LOGGING_H

/**
 * Type defining the different log levels in use in the app.
 */
typedef enum log_level {
    Verbose = 1, /**< Print all messages */
    Debug = 2, /**< Print all messages except verbose */
    Info = 3, /**< Print all messages except verbose and debug*/
    Warning = 4, /**< Print only warning and error messages */
    Error = 5, /**< Print only error messages */
    None = 6 /** Don't print anything at all */
} log_level;

/**
 * Prints a verbose message if the current log level includes verbose messages,
 * and returns the number of characters printed (0 if the current log level does
 * not include verbose messages)
 *
 * @param format The format to use to display the message
 * @param ...    The arguments to use to format the message
 *
 * @return the number of character printed.
 */
int logv(char *format, ...);

/**
 * Prints a debug message if the current log level includes debug messages, and
 * returns the number of characters printed (0 if the current log level does
 * not include debug messages)
 *
 * @param format The format to use to display the message
 * @param ...    The arguments to use to format the message
 *
 * @return the number of character printed.
 */
int logd(char *format, ...);

/**
 * Prints an info message if the current log level includes info messages, and
 * returns the number of characters printed (0 if the current log level does not
 * include info messages)
 *
 * @param format The format to use to display the message
 * @param ...    The arguments to use to format the message
 *
 * @return the number of character printed.
 */
int logi(char *format, ...);

/**
 * Prints a warning message if the current log level includes warning messages,
 * and returns the number of characters printed (0 if the current log level does
 * not include warning messages)
 *
 * @param format The format to use to display the message
 * @param ...    The arguments to use to format the message
 *
 * @return the number of character printed.
 */
int logw(char *format, ...);

/**
 * Prints an error message if the current log level includes error messages, and
 * returns the number of characters printed (0 if the current log level does not
 * include error messages)
 *
 * @param format The format to use to display the message
 * @param ...    The arguments to use to format the message
 *
 * @return the number of character printed.
 */
int loge(char *format, ...);

/**
 * Sets the current log level of the application.
 *
 * @param new_level The log level to use in the application
 */
void set_log_level(log_level new_level);

#endif  /* LOGGING_LOGGING_H */