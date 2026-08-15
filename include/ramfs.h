#ifndef RAMFS_H
#define RAMFS_H

#define RAMFS_MAX_FILES 32
#define RAMFS_FILENAME_SIZE 32
#define RAMFS_FILE_SIZE 512

typedef struct
{
    char name[RAMFS_FILENAME_SIZE];
    char data[RAMFS_FILE_SIZE];
    unsigned int size;
    int used;
} ramfs_file_t;

void ramfs_init(void);
int ramfs_create(const char *name);
int ramfs_write(const char *name, const char *data);
const char *ramfs_read(const char *name);
void ramfs_list(void);

#endif