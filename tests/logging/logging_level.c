#include <stdio.h>
#include <stdlib.h>
#include "../../src/logging/logging.h"

int main(int argc, char *argv[]) {
    set_log_level(None);
    loge("Level None: Error\n");
    logw("Level None: Warning\n");
    logi("Level None: Info\n");
    logd("Level None: Debug\n");
    logv("Level None: Verbose\n");
    set_log_level(Error);
    loge("Level Error: Error\n");
    logw("Level Error: Warning\n");
    logi("Level Error: Info\n");
    logd("Level Error: Debug\n");
    logv("Level Error: Verbose\n");
    set_log_level(Warning);
    loge("Level Warning: Error\n");
    logw("Level Warning: Warning\n");
    logi("Level Warning: Info\n");
    logd("Level Warning: Debug\n");
    logv("Level Warning: Verbose\n");
    set_log_level(Info);
    loge("Level Info: Error\n");
    logw("Level Info: Warning\n");
    logi("Level Info: Info\n");
    logd("Level Info: Debug\n");
    logv("Level Info: Verbose\n");
    set_log_level(Debug);
    loge("Level Debug: Error\n");
    logw("Level Debug: Warning\n");
    logi("Level Debug: Info\n");
    logd("Level Debug: Debug\n");
    logv("Level Debug: Verbose\n");
    set_log_level(Verbose);
    loge("Level Verbose: Error\n");
    logw("Level Verbose: Warning\n");
    logi("Level Verbose: Info\n");
    logd("Level Verbose: Debug\n");
    logv("Level Verbose: Verbose\n");
    return EXIT_SUCCESS;
}