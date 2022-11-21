#ifndef UTILS_MEMORY_UTILS_H
#define UTILS_MEMORY_UTILS_H

void *allocate(size_t memory_size);
FILE *open_file(char *file_path, char *mode);
void create_directory_if_not_exists(char *directory);

#endif  /* UTILS_MEMORY_UTILS_H */
