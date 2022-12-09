#include "config_utils.h"
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#ifndef _WIN32
#include <pwd.h>
#endif
#include "../logging/logging.h"

static const char* destination_prefix = "destination.";

static void parse_current_char(
    char current,
    int *status,
    int *index,
    int *destination_index,
    bool *is_still_good,
    bool *is_finished,
    char *folder,
    char *destination
) {
    if(current == '\r' || current == '\n') {
        if(destination_index[0] != 0) {
            is_finished[0] = true;
        } else {
            index[0] = 0;
            is_still_good[0] = true;
            status[0] = 0;
        }
    } else if(is_still_good[0]) {
        if(status[0] == 0) {
            if(current != '\t' && current != ' ') {
                status[0] = 1;
            }
        }
        
        if(status[0] == 1) {
            if(index[0] < 12 && current == destination_prefix[index[0]]) {
                index[0]++;
            } else if(index[0] >= 12 && current == destination[index[0] - 12]) {
                if(destination[index[0] - 11] == 0x00) {
                    status[0] = 2;
                }
                index[0]++;
            } else {
                is_still_good[0] = false;
            }
        } else if(status[0] == 2) {
            // If it's a tab, a space or :
            if(current == 0x09 || current == 0x20) {
                //Simply ignore it
            } else if(current == ':') {
                status[0] = 3;
            }
        } else if(status[0] == 3) {
            if(current == '\t' || current == ' ') {
                //Simply ignore it
            } else {
                status[0] = 4;
            }
        }
        
        if(status[0] == 4) {
            if(destination_index[0] < 4096) {
                folder[destination_index[0]] = current;
                destination_index[0]++;
            }
        }
    }
}

void set_destination(char *destination_name, char* folder) {
#ifdef _WIN32
    size_t pointer_size = sizeof(char) * (strlen(getenv("USERPROFILE")) + 18);
    char *user_config_file_path = malloc(pointer_size);
    if(user_config_file_path == NULL) {
        loge("Failed to allocate memory for destination config file path");
        exit(EXIT_FAILURE);
    }
    sprintf(
        user_config_file_path,
        "%s/.droidimg.config",
        getenv("USERPROFILE")
    );
    free(user_config_file_path);
#else
    struct passwd *pw = getpwuid(getuid());
    size_t pointer_size = sizeof(char) * (strlen(pw->pw_dir) + 18);
    char *user_config_file_path = malloc(pointer_size);
    if(user_config_file_path == NULL) {
        loge("Failed to allocate memory for destination config file path");
        exit(EXIT_FAILURE);
    }
    sprintf(user_config_file_path, "%s/.droidimg.config", pw->pw_dir);
    free(pw);
#endif
    FILE *file_pointer = fopen(user_config_file_path, "rt");
    if(file_pointer == NULL) {
        fprintf(stderr, "Failed open config file. ");
        fprintf(stderr, "Ignoring destination argument.\n");
        fprintf(stderr, "Error was %s.\n", strerror(errno));
    }
    // 0 : wait for destination
    // 1 : parsing destination name
    // 2 : end parsing destination name (expecting :)
    // 3 : found : (expecting folder)
    // 4 : parsing folder
    int status = 0;
    int index = 0;
    int destination_index = 0;
    bool is_still_good = true;
    bool is_finished = false;
    char *current = malloc(sizeof(char) * 2);
    if(current == NULL) {
        loge("Failed to allocate memory for destination buffer");
        exit(EXIT_FAILURE);
    }
    while(fgets(current, sizeof(char) * 2, file_pointer) && !is_finished) {
        parse_current_char(
            current[0],
            &status,
            &index,
            &destination_index,
            &is_still_good,
            &is_finished,
            folder,
            destination_name
        );
    }
    free(current);
    if(folder != NULL && folder[destination_index-1] != 0x00) {
        folder[destination_index] = 0x00;
    }
    fclose(file_pointer);
}