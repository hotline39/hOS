#include "fat12.h"
#include "vga.h"

extern unsigned char _binary_fat12_img_start[];
extern unsigned char _binary_fat12_img_end[];

static fat12_fs_t fs;

static unsigned short read16(const unsigned char *p)
{
    return (unsigned short)p[0] |
           ((unsigned short)p[1] << 8);
}

static unsigned int read32(const unsigned char *p)
{
    return (unsigned int)p[0] |
           ((unsigned int)p[1] << 8) |
           ((unsigned int)p[2] << 16) |
           ((unsigned int)p[3] << 24);
}

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

static void make_fat_name(const char *input, char *output)
{
    unsigned int i = 0;
    unsigned int j = 0;

    while (input[i] != '\0' && input[i] != '.' && j < 8)
    {
        output[j++] = input[i++];
    }

    while (j < 8)
        output[j++] = ' ';

    if (input[i] == '.')
        i++;

    while (input[i] != '\0' && j < 11)
        output[j++] = input[i++];

    while (j < 11)
        output[j++] = ' ';

    output[11] = '\0';
}

static unsigned short fat12_next_cluster(unsigned short cluster)
{
    unsigned int offset;
    unsigned int value;

    offset = fs.fat_start * fs.bytes_per_sector +
             cluster + (cluster / 2);

    value = read16(fs.image + offset);

    if (cluster & 1)
        value >>= 4;
    else
        value &= 0x0FFF;

    return (unsigned short)value;
}

static unsigned char *cluster_address(unsigned short cluster)
{
    unsigned int sector;

    sector = fs.data_start +
             ((cluster - 2) * fs.sectors_per_cluster);

    return fs.image + sector * fs.bytes_per_sector;
}

void fat12_init(void)
{
    unsigned int size;

    fs.image = 0;
    fs.image_size = 0;

    size = (unsigned int)(
        _binary_fat12_img_end -
        _binary_fat12_img_start
    );

    if (fat12_mount(_binary_fat12_img_start, size) == 0)
        vga_write("FAT12 mounted!\n");
    else
        vga_write("FAT12 mount failed!\n");
}

int fat12_mount(unsigned char *image, unsigned int image_size)
{
    unsigned char *boot;

    if (image == 0)
        return -1;

    if (image_size < FAT12_SECTOR_SIZE)
        return -1;

    boot = image;

    fs.image = image;
    fs.image_size = image_size;

    fs.bytes_per_sector = read16(boot + 11);
    fs.sectors_per_cluster = boot[13];
    fs.reserved_sectors = read16(boot + 14);
    fs.fat_count = boot[16];
    fs.root_entries = read16(boot + 17);

    fs.total_sectors = read16(boot + 19);

    if (fs.total_sectors == 0)
        fs.total_sectors = read32(boot + 32);

    fs.sectors_per_fat = read16(boot + 22);

    if (fs.bytes_per_sector == 0 ||
        fs.sectors_per_cluster == 0 ||
        fs.fat_count == 0 ||
        fs.sectors_per_fat == 0)
    {
        return -1;
    }

    fs.fat_start = fs.reserved_sectors;

    fs.root_start =
        fs.fat_start +
        (fs.fat_count * fs.sectors_per_fat);

    fs.data_start =
        fs.root_start +
        ((fs.root_entries * 32 + fs.bytes_per_sector - 1) /
         fs.bytes_per_sector);

    return 0;
}

void fat12_list(void)
{
    unsigned int i;
    unsigned int root_size;
    unsigned char *entry;

    if (fs.image == 0)
    {
        vga_write("FAT12 not mounted.\n");
        return;
    }

    root_size = fs.root_entries * 32;

    for (i = 0; i < root_size; i += 32)
    {
        entry = fs.image +
                fs.root_start * fs.bytes_per_sector +
                i;

        if (entry[0] == 0x00)
            break;

        if (entry[0] == 0xE5)
            continue;

        if (entry[11] == 0x0F)
            continue;

        if (entry[11] & 0x08)
            continue;

        vga_write("FAT12: ");

        {
            char name[13];
            unsigned int j;
            unsigned int pos = 0;

            for (j = 0; j < 8; j++)
            {
                if (entry[j] == ' ')
                    break;

                name[pos++] = entry[j];
            }

            if (entry[8] != ' ')
            {
                name[pos++] = '.';

                for (j = 8; j < 11; j++)
                {
                    if (entry[j] == ' ')
                        break;

                    name[pos++] = entry[j];
                }
            }

            name[pos] = '\0';

            vga_write(name);
            vga_write("\n");
        }
    }
}

const char *fat12_read(const char *name)
{
    static char buffer[4096];

    char fat_name[12];

    unsigned int i;
    unsigned int root_size;

    unsigned short cluster;
    unsigned int file_size;
    unsigned int copied = 0;

    unsigned char *entry;

    if (fs.image == 0)
        return 0;

    make_fat_name(name, fat_name);

    root_size = fs.root_entries * 32;

    for (i = 0; i < root_size; i += 32)
    {
        entry = fs.image +
                fs.root_start * fs.bytes_per_sector +
                i;

        if (entry[0] == 0x00)
            break;

        if (entry[0] == 0xE5)
            continue;

        if (entry[11] == 0x0F)
            continue;

        if (entry[11] & 0x08)
            continue;

        {
            unsigned int j;
            int match = 1;

            for (j = 0; j < 11; j++)
            {
                if (entry[j] != (unsigned char)fat_name[j])
                {
                    match = 0;
                    break;
                }
            }

            if (!match)
                continue;
        }

        cluster = read16(entry + 26);
        file_size = read32(entry + 28);

        while (cluster >= 2 &&
               cluster < 0xFF8 &&
               copied < file_size &&
               copied < sizeof(buffer) - 1)
        {
            unsigned char *data;
            unsigned int bytes;

            data = cluster_address(cluster);

            bytes = fs.sectors_per_cluster *
                    fs.bytes_per_sector;

            if (bytes > file_size - copied)
                bytes = file_size - copied;

            if (bytes > sizeof(buffer) - 1 - copied)
                bytes = sizeof(buffer) - 1 - copied;

            for (int j = 0; j < bytes; j++)
                buffer[copied + j] = data[j];

            copied += bytes;

            cluster = fat12_next_cluster(cluster);
        }

        buffer[copied] = '\0';

        return buffer;
    }

    return 0;
}