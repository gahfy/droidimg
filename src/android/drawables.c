#include "drawables.h"
#include <stdlib.h>
#include <stdio.h>
#if defined HAVE_LIBPTHREAD && HAVE_LIBPTHREAD == 1
    #include <pthread.h>
#endif
#include <string.h>
#include "../files/writer.h"
#include "../logging/logging.h"
#include "../webp/writer.h"

typedef enum drawable_res {
    ldpi,
    mdpi,
    hdpi,
    xhdpi,
    xxhdpi,
    xxxhdpi
} drawable_res;

typedef struct thread_arg{
    picture *picture_pointer;
    char *output_folder;
    drawable_config *config;
    int thread_id;
} thread_arg;

static thread_arg *init_thread_arguments(
    picture *restrict picture_pointer,
    char *restrict output_folder,
    drawable_config *restrict config
);
static void *write_ldpi(void *args);
static void *write_mdpi(void *args);
static void *write_hdpi(void *args);
static void *write_xhdpi(void *args);
static void *write_xxhdpi(void *args);
static void *write_xxxhdpi(void *arguments);
static void validate_thread_arguments(thread_arg *restrict arguments);

static char *get_and_create_destination_folder(
    char *restrict output_folder, char *restrict drawable_folder
);

static void validate_destination_folder(char *restrict destination_folder);

static void set_destination_folder(
    char *restrict destination_folder,
    char *restrict output_folder,
    char *restrict drawable_folder
);

static void write_android_drawable(
    char *restrict destination_folder,
    picture *restrict picture_pointer,
    drawable_config *restrict config,
    drawable_res resolution
);

static uint32_t get_drawable_size(uint32_t mdpi_size, drawable_res resolution);

static char *get_file_path(char *restrict folder, char *restrict name);

static void validate_file_path(char *restrict file_path);

#if defined HAVE_LIBPTHREAD && HAVE_LIBPTHREAD == 1
    static pthread_t *init_threads();
    static void create_threads(
        pthread_t *restrict threads, thread_arg *restrict arguments
    );
    static void join_threads_and_free(
        pthread_t *restrict threads, thread_arg *restrict arguments
    );
#else
    static void convert_without_threads(thread_arg *restrict arguments);
#endif

void write_android_drawables(
    picture *restrict image,
    char *restrict output_folder,
    drawable_config *restrict config
) {
    thread_arg *arguments = init_thread_arguments(image, output_folder, config);
    #if defined HAVE_LIBPTHREAD && HAVE_LIBPTHREAD == 1
        pthread_t *threads = init_threads();
        create_threads(threads, arguments);
        join_threads_and_free(threads, arguments);
    #else
        convert_without_threads(arguments);
        free(arguments);
    #endif
}

#if defined HAVE_LIBPTHREAD && HAVE_LIBPTHREAD == 1
    static void create_threads(
        pthread_t *restrict threads, thread_arg *restrict arguments
    ) {
        pthread_create(&threads[0], NULL, write_ldpi, (void *) arguments);
        pthread_create(&threads[1], NULL, write_mdpi, (void *) arguments);
        pthread_create(&threads[2], NULL, write_hdpi, (void *) arguments);
        pthread_create(&threads[3], NULL, write_xhdpi, (void *) arguments);
        pthread_create(&threads[4], NULL, write_xxhdpi, (void *) arguments);
        pthread_create(&threads[5], NULL, write_xxxhdpi, (void *) arguments);
    }

    /**
     * Initializes the threads that will be used to write the drawables. If
     * memory allocation failed, the program will end with an error message.
     *
     * @return the pointer to the threads that has been initialized
     */
    static pthread_t *init_threads() {
        pthread_t *threads = malloc(sizeof(pthread_t) * 6);
        if(threads == NULL) {
            loge("Failed to allocate memory for threads");
            exit(EXIT_FAILURE);
        }
        return threads;
    }

    /**
    * Joins the threads in the given pointer and free all allocated memory for
    * the threads.
    *
    * @param threads   the threads to join and then free
    * @param arguments the arguments to free at the end
    */
    static void join_threads_and_free(
        pthread_t *restrict threads, thread_arg *restrict arguments
    ) {
        for (int i=0; i<6; ++i) {
            pthread_join(threads[i], NULL);
        }
        free(threads);
        free(arguments);
    }
#else
    static void convert_without_threads(thread_arg *restrict arguments) {
        write_ldpi(arguments);
        write_mdpi(arguments);
        write_hdpi(arguments);
        write_xhdpi(arguments);
        write_xxhdpi(arguments);
        write_xxxhdpi(arguments);
    }
#endif

/**
 * Initializes a pointer to the structure which will contain the arguments
 * passed to each thread function.
 *
 * @param picture_pointer the pointer to the picture to write
 * @param output_folder   the resources folder in which to write the drawables
 * @param config          the configuration that will be used to write the
 *                        drawables
 *
 * @return the pointer to the arguments that has been initialized.
 */
static thread_arg *init_thread_arguments(
    picture *restrict picture_pointer,
    char *restrict output_folder,
    drawable_config *restrict config
) {
    thread_arg *arguments = malloc(sizeof(thread_arg));
    validate_thread_arguments(arguments);
    arguments->output_folder = output_folder;
    arguments->config = config;
    arguments->picture_pointer = picture_pointer;
    return arguments;
}

static void *write_ldpi(void *args) {
    printf("Start ldpi\n");
    thread_arg *arguments = (thread_arg *) args;
    char *destination_folder = get_and_create_destination_folder(
        arguments->output_folder, "drawable-ldpi"
    );
    write_android_drawable(
        destination_folder,
        arguments->picture_pointer,
        arguments->config,
        ldpi
    );
    free(destination_folder);
    printf("Ended ldpi\n");
    return NULL;
}

static void *write_mdpi(void *args) {
    printf("Start mdpi\n");
    thread_arg *arguments = (thread_arg *) args;
    char *destination_folder = get_and_create_destination_folder(
        arguments->output_folder, "drawable-mdpi"
    );
    write_android_drawable(
        destination_folder,
        arguments->picture_pointer,
        arguments->config,
        mdpi
    );
    free(destination_folder);
    printf("Ended mdpi\n");
    return NULL;
}

static void *write_hdpi(void *args) {
    printf("Start hdpi\n");
    thread_arg *arguments = (thread_arg *) args;
    char *destination_folder = get_and_create_destination_folder(
        arguments->output_folder, "drawable-hdpi"
    );
    write_android_drawable(
        destination_folder,
        arguments->picture_pointer,
        arguments->config,
        hdpi
    );
    free(destination_folder);
    printf("Ended hdpi\n");
    return NULL;
}

static void *write_xhdpi(void *args) {
    printf("Start xhdpi\n");
    thread_arg *arguments = (thread_arg *) args;
    char *destination_folder = get_and_create_destination_folder(
        arguments->output_folder, "drawable-xhdpi"
    );
    write_android_drawable(
        destination_folder,
        arguments->picture_pointer,
        arguments->config,
        xhdpi
    );
    free(destination_folder);
    printf("Ended xhdpi\n");
    return NULL;
}

static void *write_xxhdpi(void *args) {
    printf("Start xxhdpi\n");
    thread_arg *arguments = (thread_arg *) args;
    char *destination_folder = get_and_create_destination_folder(
        arguments->output_folder, "drawable-xxhdpi"
    );
    write_android_drawable(
        destination_folder,
        arguments->picture_pointer,
        arguments->config,
        xxhdpi
    );
    free(destination_folder);
    printf("Ended xxhdpi\n");
    return NULL;
}

static void *write_xxxhdpi(void *args) {
    printf("Start xxxhdpi\n");
    thread_arg *arguments = (thread_arg *) args;
    char *destination_folder = get_and_create_destination_folder(
        arguments->output_folder, "drawable-xxxhdpi"
    );
    write_android_drawable(
        destination_folder,
        arguments->picture_pointer,
        arguments->config,
        xxxhdpi
    );
    free(destination_folder);
    printf("Ended xxxhdpi\n");
    return NULL;
}

/**
 * Checks that the given arguments is not NULL. If it is, then the program will
 * end with an error message.
 *
 * @param arguments the arguments to check
 */
static void validate_thread_arguments(thread_arg *restrict arguments) {
    if(arguments == NULL) {
        loge("Failed to allocate memory for arguments");
        exit(EXIT_FAILURE);
    }
}

static char *get_and_create_destination_folder(
    char *restrict output_folder, char *restrict drawable_folder
) {
    int output_folder_length = strlen(output_folder);
    int drawable_folder_length = strlen(drawable_folder);
    int destination_length = output_folder_length + drawable_folder_length;
    char *destination_folder = malloc(sizeof(char) * (destination_length + 1));
    validate_destination_folder(destination_folder);
    set_destination_folder(destination_folder, output_folder, drawable_folder);
    destination_folder[destination_length] = 0x00;
    create_directory_if_not_exists(destination_folder);
    return destination_folder;
}

static void validate_destination_folder(char *restrict destination_folder) {
    if(destination_folder == NULL) {
        loge("Failed to allocate memory for the name of the directory");
        exit(EXIT_FAILURE);
    }
}

static void set_destination_folder(
    char *restrict destination_folder,
    char *restrict output_folder,
    char *restrict drawable_folder
) {
    int output_folder_length = strlen(output_folder);
    int drawable_folder_length = strlen(drawable_folder);
    int destination_length = output_folder_length + drawable_folder_length;
    strncpy(destination_folder, output_folder, output_folder_length);
    strncpy(
        &destination_folder[output_folder_length],
        drawable_folder,
        drawable_folder_length
    );
}

static void write_android_drawable(
    char *restrict destination_folder,
    picture *restrict picture_pointer,
    drawable_config *restrict config,
    drawable_res resolution
) {
    uint32_t width = get_drawable_size(config->width, resolution);
    uint32_t height = get_drawable_size(config->height, resolution);
    char *file_path = get_file_path(destination_folder, config->name);
    write_picture_to_webp(
        picture_pointer, file_path, width, height, config->quality
    );
    free(file_path);
}

static uint32_t get_drawable_size(uint32_t mdpi_size, drawable_res resolution) {
    switch(resolution) {
        case ldpi: return mdpi_size * 3 / 4;
        case mdpi: return mdpi_size;
        case hdpi: return mdpi_size * 3 / 2;
        case xhdpi: return mdpi_size * 2;
        case xxhdpi: return mdpi_size * 3;
        case xxxhdpi: return mdpi_size * 4;
    }
}

static char *get_file_path(char *restrict folder, char *restrict name) {
    int folder_length = strlen(folder);
    int name_length = strlen(name);
    int file_path_length = folder_length + name_length + 1;
    char *file_path = malloc(file_path_length+1);
    validate_file_path(file_path);
    strncpy(file_path, folder, folder_length);
    file_path[folder_length] = '/';
    strncpy(&file_path[folder_length + 1], name, name_length);
    file_path[file_path_length] = 0x00;
    return file_path;
}

static void validate_file_path(char *restrict file_path) {
    if(file_path == NULL) {
        loge("Failed to allocate memory for the name of the directory");
        exit(EXIT_FAILURE);
    }
}