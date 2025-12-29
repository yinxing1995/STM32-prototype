MEMORY
{
  rom : org = 0x08000000, l = 64K
  ram : org = 0x20000000, l = 20K
}

_stack = ORIGIN(ram) + LENGTH(ram);

SECTIONS  
{
  .text : 
  {
    ld_text_begin = .;
    KEEP(*(.text.vector_table));
    KEEP(*(.text.crt0));
    *(.text.*);
    ld_text_end = .;
  } > rom
  ld_text_size = ld_text_end - ld_text_begin;

  .bss :
  {
    ld_bss_begin = .;
    *(.bss);
    *(.bss.*);
    ld_bss_end = .;
  } > ram
  ld_bss_size = ld_bss_end - ld_bss_begin;

  .data : AT(lma_rom_begin)
  {
    ld_data_begin = .;
    *(.data);
    *(.data.*);
    ld_data_end = .;
  } > ram

  ld_data_size = ld_data_end - ld_data_begin;

  .lma : 
  {
    lma_rom_begin = .;
    . += ld_data_size;
    lma_rom_end = .;
  } > rom
}
