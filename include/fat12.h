#ifndef FAT12_H
#define FAT12_H

#define FAT12_SECTOR_SIZE 512

typedef struct
{
    unsigned char *image;
    unsigned int image_size;

    unsigned int bytes_per_sector;
    unsigned int sectors_per_cluster;
    unsigned int reserved_sectors;
    unsigned int fat_count;
    unsigned int root_entries;
    unsigned int total_sectors;
    unsigned int sectors_per_fat;

    unsigned int fat_start;
    unsigned int root_start;
    unsigned int data_start;
} fat12_fs_t;

void fat12_init(void);
int fat12_mount(unsigned char *image, unsigned int image_size);
void fat12_list(void);
const char *fat12_read(const char *name);

#endif