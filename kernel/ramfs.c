#include "ramfs.h"
#include "vga.h"

static ramfs_file_t files[RAMFS_MAX_FILES];

static int string_equal(const char *a, const char *b)
{
    unsigned int i = 0;

    while (a[i] != '\0' && b[i] != '\0')
    {
        if (a[i] != b[i])
            return 0;

        i++;
    }

    return a[i] == '\0' && b[i] == '\0';
}

static void string_copy(char *dest, const char *src)
{
    unsigned int i = 0;

    while (src[i] != '\0' && i < RAMFS_FILENAME_SIZE - 1)
    {
        dest[i] = src[i];
        i++;
    }

    dest[i] = '\0';
}

void ramfs_init(void)
{
    unsigned int i;

    for (i = 0; i < RAMFS_MAX_FILES; i++)
    {
        files[i].used = 0;
        files[i].size = 0;
        files[i].name[0] = '\0';
        files[i].data[0] = '\0';
    }

    vga_write("RAM FS initialized!\n");
}

int ramfs_create(const char *name)
{
    unsigned int i;

    for (i = 0; i < RAMFS_MAX_FILES; i++)
    {
        if (files[i].used && string_equal(files[i].name, name))
            return -1;
    }

    for (i = 0; i < RAMFS_MAX_FILES; i++)
    {
        if (!files[i].used)
        {
            files[i].used = 1;
            files[i].size = 0;
            string_copy(files[i].name, name);
            files[i].data[0] = '\0';

            return 0;
        }
    }

    return -1;
}

int ramfs_write(const char *name, const char *data)
{
    unsigned int i;
    unsigned int j;

    for (i = 0; i < RAMFS_MAX_FILES; i++)
    {
        if (files[i].used && string_equal(files[i].name, name))
        {
            for (j = 0; j < RAMFS_FILE_SIZE - 1 && data[j] != '\0'; j++)
                files[i].data[j] = data[j];

            files[i].data[j] = '\0';
            files[i].size = j;

            return 0;
        }
    }

    return -1;
}

const char *ramfs_read(const char *name)
{
    unsigned int i;

    for (i = 0; i < RAMFS_MAX_FILES; i++)
    {
        if (files[i].used && string_equal(files[i].name, name))
            return files[i].data;
    }

    return 0;
}

void ramfs_list(void)
{
    unsigned int i;

    for (i = 0; i < RAMFS_MAX_FILES; i++)
    {
        if (files[i].used)
        {
            vga_write(files[i].name);
            vga_write("\n");
        }
    }
}