ENTRY(_start)

PHDRS
{
    text PT_LOAD FLAGS(5);
    rodata PT_LOAD FLAGS(4);
    data PT_LOAD FLAGS(6);
}

SECTIONS
{
    . = 1M;

    .text ALIGN(4096) :
    {
        *(.multiboot)
        *(.text)
        *(.text.*)
    } :text

    .rodata ALIGN(4096) :
    {
        *(.rodata)
        *(.rodata.*)
        *(.eh_frame)
        *(.eh_frame.*)
    } :rodata

    .data ALIGN(4096) :
    {
        *(.data)
        *(.data.*)
    } :data

    .bss ALIGN(4096) :
    {
        *(COMMON)
        *(.bss)
        *(.bss.*)
    } :data

    /DISCARD/ :
    {
        *(.comment)
        *(.note.GNU-stack)
    }
}