#include "elf.h"
#include "vga.h"

static void memory_copy(
    unsigned char *dest,
    const unsigned char *src,
    unsigned int size
)
{
    unsigned int i;

    for (i = 0; i < size; i++)
        dest[i] = src[i];
}

static void memory_zero(
    unsigned char *dest,
    unsigned int size
)
{
    unsigned int i;

    for (i = 0; i < size; i++)
        dest[i] = 0;
}

int elf_check(const unsigned char *data)
{
    elf32_header_t *header;

    if (data == 0)
        return 0;

    header = (elf32_header_t *)data;

    if (header->e_ident[0] != 0x7F)
        return 0;

    if (header->e_ident[1] != 'E')
        return 0;

    if (header->e_ident[2] != 'L')
        return 0;

    if (header->e_ident[3] != 'F')
        return 0;

    if (header->e_ident[4] != 1)
        return 0;

    if (header->e_ident[5] != 1)
        return 0;

    if (header->e_machine != 3)
        return 0;

    if (header->e_phoff == 0)
        return 0;

    if (header->e_phnum == 0)
        return 0;

    if (header->e_phentsize != sizeof(elf32_program_header_t))
        return 0;

    return 1;
}

unsigned int elf_load(const unsigned char *data)
{
    elf32_header_t *header;
    elf32_program_header_t *program_header;
    unsigned int i;

    if (!elf_check(data))
    {
        vga_write("ELF: invalid ELF file\n");
        return 0;
    }

    header = (elf32_header_t *)data;

    for (i = 0; i < header->e_phnum; i++)
    {
        program_header =
            (elf32_program_header_t *)
            (data + header->e_phoff +
             i * header->e_phentsize);

        if (program_header->p_type != ELF_PT_LOAD)
            continue;

        if (program_header->p_filesz > program_header->p_memsz)
        {
            vga_write("ELF: invalid segment\n");
            return 0;
        }

        if (program_header->p_memsz == 0)
            continue;

        memory_copy(
            (unsigned char *)program_header->p_vaddr,
            data + program_header->p_offset,
            program_header->p_filesz
        );

        if (program_header->p_memsz > program_header->p_filesz)
        {
            memory_zero(
                (unsigned char *)(
                    program_header->p_vaddr +
                    program_header->p_filesz
                ),
                program_header->p_memsz -
                program_header->p_filesz
            );
        }
    }

    vga_write("ELF loaded!\n");

    return header->e_entry;
}