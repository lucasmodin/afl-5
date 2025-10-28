#ifndef FILESYS_CONFIG_H
#define FILESYS_CONFIG_H
#include <stddef.h>

struct setting_t {
    char *name;
    char *value;
};

// TODO: Ret `char *` til at være `struct setting_t *`
struct config_t {
    // struct setting_t *lines[20];
    struct setting_t *lines[20];
    size_t count;
};

struct config_t *read_config(char *filename);
void print_config(struct config_t *config);
void free_config(struct config_t *config);

void print_setting(struct setting_t *setting);
struct setting_t *setting_converter(char *line);

#endif //FILESYS_CONFIG_H