ENTRY(K_Init)

SECTIONS
{
  . = 1M;
  .boot   : { PROVIDE(__boot_start = .);    *(.boot)    }
  .text   : { PROVIDE(__text_start = .);    *(.text)    }
  .data   : { PROVIDE(__data_start = .);    *(.data)    }
  .rodata : { PROVIDE(__rodata_start = .);  *(.rodata)  }
  .bss    : { PROVIDE(__bss_start = .);     *(.bss)     }
  PROVIDE(__end = .);
}
