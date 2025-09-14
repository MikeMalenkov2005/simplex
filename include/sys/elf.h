#ifndef _SYS_ELF_H
#define _SYS_ELF_H

#include <types.h>

typedef K_U32 Elf32_Addr;
typedef K_U32 Elf32_Off;
typedef K_U16 Elf32_Half;
typedef K_U32 Elf32_Word;
typedef K_S32 Elf32_Sword;

typedef K_U64S  Elf64_Addr;
typedef K_U64S  Elf64_Off;
typedef K_U16   Elf64_Half;
typedef K_U32   Elf64_Word;
typedef K_S32   Elf64_Sword;
typedef K_U64S  Elf64_Xword;
typedef K_S64S  Elf64_Sxword;

#define EI_MAG0       0
#define EI_MAG1       1
#define EI_MAG2       2
#define EI_MAG3       3
#define EI_CLASS      4
#define EI_DATA       5
#define EI_VERSION    6
#define EI_OSABI      7
#define EI_ABIVERSION 8
#define EI_PAD        9
#define EI_NIDENT     16

#define ELFMAG0 0x7F
#define ELFMAG1 'E'
#define ELFMAG2 'L'
#define ELFMAG3 'F'

#define ELFCLASSNONE  0
#define ELFCLASS32    1
#define ELFCLASS64    2

#define ELFDATANONE 0
#define ELFDATA2LSB 1
#define ELFDATA2MSB 2

#define EV_NONE     0
#define EV_CURRENT  1

#define ELFOSABI_NONE   0
#define ELFOSABI_HPUX   1
#define ELFOSABI_NETBSD 2
#define ELFOSABI_LINUX  3
#define ELFOSABI_ARM    97

#define ET_NONE   0
#define ET_REL    1
#define ET_EXEC   2
#define ET_DYN    3
#define ET_CORE   4
#define ET_LOOS   0xFE00
#define ET_HIOS   0xFEFF
#define ET_LOPROC 0xFF00
#define ET_HIPROC 0xFFFF

#define EM_NONE   0
#define EM_386    3
#define EM_ARM    40
#define EM_X86_64 62

struct Elf32_Ehdr
{
  K_U8 e_ident[EI_NIDENT];
  Elf32_Half  e_type;
  Elf32_Half  e_machine;
  Elf32_Word  e_version;
  Elf32_Addr  e_entry;
  Elf32_Off   e_phoff;
  Elf32_Off   e_shoff;
  Elf32_Word  e_flags;
  Elf32_Half  e_ehsize;
  Elf32_Half  e_phentsize;
  Elf32_Half  e_phnum;
  Elf32_Half  e_shentsize;
  Elf32_Half  e_shnum;
  Elf32_Half  e_shstrndx;
};

struct Elf64_Ehdr
{
  K_U8 e_ident[EI_NIDENT];
  Elf64_Half  e_type;
  Elf64_Half  e_machine;
  Elf64_Word  e_version;
  Elf64_Addr  e_entry;
  Elf64_Off   e_phoff;
  Elf64_Off   e_shoff;
  Elf64_Word  e_flags;
  Elf64_Half  e_ehsize;
  Elf64_Half  e_phentsize;
  Elf64_Half  e_phnum;
  Elf64_Half  e_shentsize;
  Elf64_Half  e_shnum;
  Elf64_Half  e_shstrndx;
};

#define SHN_UNDEF     0
#define SHN_LORESERVE 0xff00
#define SHN_LOPROC    0xff00
#define SHN_HIPROC    0xff1f
#define SHN_LOOS      0xff20
#define SHN_HIOS      0xff3f
#define SHN_ABS       0xfff1
#define SHN_COMMON    0xfff2
#define SHN_XINDEX    0xffff
#define SHN_HIRESERVE 0xffff

#define SHT_NULL                0
#define SHT_PROGBITS            1
#define SHT_SYMTAB              2
#define SHT_STRTAB              3
#define SHT_RELA                4
#define SHT_HASH                5
#define SHT_DYNAMIC             6
#define SHT_NOTE                7
#define SHT_NOBITS              8
#define SHT_REL                 9
#define SHT_SHLIB               10
#define SHT_DYNSYM              11
#define SHT_INIT_ARRAY          14
#define SHT_FINI_ARRAY          15
#define SHT_PREINIT_ARRAY       16
#define SHT_GROUP               17
#define SHT_SYMTAB_SHNDX        18
#define SHT_LOOS                0x60000000
#define SHT_HIOS                0x6fffffff
#define SHT_LOPROC              0x70000000
#define SHT_HIPROC              0x7fffffff
#define SHT_LOUSER              0x80000000
#define SHT_HIUSER              0xffffffff

#define SHT_ARM_EXIDX           0x70000001
#define SHT_ARM_PREEMPTMAP      0x70000002
#define SHT_ARM_ATTRIBUTES      0x70000003
#define SHT_ARM_DEBUGOVERLAY    0x70000004
#define SHT_ARM_OVERLAYSECTION  0x70000005
#define SHT_HEX_ORDERED         0x70000000

#define SHT_X86_64_UNWIND       0x70000001

#define SHF_WRITE             0x1
#define SHF_ALLOC             0x2
#define SHF_EXECINSTR         0x4
#define SHF_MERGE             0x10
#define SHF_STRINGS           0x20
#define SHF_INFO_LINK         0x40
#define SHF_LINK_ORDER        0x80
#define SHF_OS_NONCONFORMING  0x100
#define SHF_GROUP             0x200
#define SHF_TLS               0x400
#define SHF_EXCLUDE           0x80000000
#define XCORE_SHF_CP_SECTION  0x800
#define XCORE_SHF_DP_SECTION  0x1000
#define SHF_MASKOS            0x0ff00000
#define SHF_MASKPROC          0xf0000000
#define SHF_X86_64_LARGE      0x10000000

#define GRP_COMDAT    0x1
#define GRP_MASKOS    0x0ff00000
#define GRP_MASKPROC  0xf0000000

struct Elf32_Shdr
{
  Elf32_Word  sh_name;
  Elf32_Word  sh_type;
  Elf32_Word  sh_flags;
  Elf32_Addr  sh_addr;
  Elf32_Off   sh_offset;
  Elf32_Word  sh_size;
  Elf32_Word  sh_link;
  Elf32_Word  sh_info;   
  Elf32_Word  sh_addralign;
  Elf32_Word  sh_entsize;
};

struct Elf64_Shdr
{
  Elf64_Word  sh_name;
  Elf64_Word  sh_type;
  Elf64_Xword sh_flags;
  Elf64_Addr  sh_addr;
  Elf64_Off   sh_offset;
  Elf64_Xword sh_size;
  Elf64_Word  sh_link;
  Elf64_Word  sh_info;   
  Elf64_Xword sh_addralign;
  Elf64_Xword sh_entsize;
};

#define SYMENTRY_SIZE32 16
#define SYMENTRY_SIZE64 24

#define STB_LOCAL   0
#define STB_GLOBAL  1
#define STB_WEAK    2
#define STB_LOOS    10
#define STB_HIOS    12
#define STB_LOPROC  13
#define STB_HIPROC  15

#define STT_NOTYPE    0
#define STT_OBJECT    1
#define STT_FUNC      2
#define STT_SECTION   3
#define STT_FILE      4
#define STT_COMMON    5
#define STT_TLS       6
#define STT_LOOS      7
#define STT_HIOS      8
#define STT_GNU_IFUNC 10
#define STT_LOPROC    13
#define STT_HIPROC    15

#define STV_DEFAULT   0
#define STV_INTERNAL  1
#define STV_HIDDEN    2
#define STV_PROTECTED 3

#define STN_UNDEF 0

struct Elf32_Sym
{
  Elf32_Word  st_name;
  Elf32_Addr  st_value;
  Elf32_Word  st_size;
  K_U8        st_info;
  K_U8        st_other;
  Elf32_Half  st_shndx;
};

struct Elf64_Sym
{
  Elf64_Word  st_name;
  K_U8        st_info;
  K_U8        st_other;
  Elf64_Half  st_shndx;
  Elf64_Addr  st_value;
  Elf64_Xword st_size;
};

/* TODO: relocations! */

#define PT_NULL         0
#define PT_LOAD         1
#define PT_DYNAMIC      2
#define PT_INTERP       3
#define PT_NOTE         4
#define PT_SHLIB        5
#define PT_PHDR         6
#define PT_TLS          7
#define PT_LOOS         0x60000000
#define PT_HIOS         0x6fffffff
#define PT_LOPROC       0x70000000
#define PT_HIPROC       0x7fffffff

#define PT_ARM_ARCHEXT  0x70000000
#define PT_ARM_EXIDX    0x70000001
#define PT_ARM_UNWIND   0x70000001

#define PF_X        1
#define PF_W        2
#define PF_R        4
#define PF_MASKOS   0x0FF00000
#define PF_MASKPROC 0xF0000000

struct Elf32_Phdr
{
  Elf32_Word  p_type;
  Elf32_Off   p_offset;
  Elf32_Addr  p_vaddr;
  Elf32_Addr  p_paddr;
  Elf32_Word  p_filesz;
  Elf32_Word  p_memsz;
  Elf32_Word  p_flags;
  Elf32_Word  p_align;
};

struct Elf64_Phdr
{
  Elf64_Word  p_type;
  Elf64_Word  p_flags;
  Elf64_Off   p_offset;
  Elf64_Addr  p_vaddr;
  Elf64_Addr  p_paddr;
  Elf64_Xword p_filesz;
  Elf64_Xword p_memsz;
  Elf64_Xword p_align;
};

/* TODO: Dynamics! */

#endif

