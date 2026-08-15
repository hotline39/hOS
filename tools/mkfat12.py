import struct

SECTOR_SIZE = 512
SECTORS = 2880

RESERVED = 1
FATS = 2
ROOT_ENTRIES = 224
SECTORS_PER_FAT = 9
SECTORS_PER_CLUSTER = 1

IMAGE_SIZE = SECTOR_SIZE * SECTORS

image = bytearray(IMAGE_SIZE)

image[0:3] = b'\xEB\x3C\x90'
image[3:11] = b'hOS FAT12'

struct.pack_into('<H', image, 11, SECTOR_SIZE)
image[13] = SECTORS_PER_CLUSTER
struct.pack_into('<H', image, 14, RESERVED)
image[16] = FATS
struct.pack_into('<H', image, 17, ROOT_ENTRIES)
struct.pack_into('<H', image, 19, SECTORS)
image[21] = 0xF0
struct.pack_into('<H', image, 22, SECTORS_PER_FAT)
struct.pack_into('<H', image, 24, 18)
struct.pack_into('<H', image, 26, 2)

fat_start = RESERVED * SECTOR_SIZE
root_start = (RESERVED + FATS * SECTORS_PER_FAT) * SECTOR_SIZE
data_start = root_start + ROOT_ENTRIES * 32

for fat in range(FATS):
    offset = fat_start + fat * SECTORS_PER_FAT * SECTOR_SIZE

    image[offset + 0] = 0xF0
    image[offset + 1] = 0xFF
    image[offset + 2] = 0xFF

content = b'Hello from FAT12!\n'

root = root_start

image[root:root + 8] = b'HELLO   '
image[root + 8:root + 11] = b'TXT'
image[root + 11] = 0x20

struct.pack_into('<H', image, root + 26, 2)
struct.pack_into('<I', image, root + 28, len(content))

image[data_start:data_start + len(content)] = content

with open('fat12.img', 'wb') as f:
    f.write(image)