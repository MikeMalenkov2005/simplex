#include "loader.h"

#include <sys/elf.h>

#if defined(__x86_64__)
#define K_EM_CUR64  EM_X86_64
#define K_EM_CUR32  EM_386
#elif defined(i386) || defined(__i386__) || defined(__i386)
#define K_EM_CUR32  EM_386
#elif defined(__arm__)
#define K_EM_CUR32  EM_ARM
#else
#error kernel/loader: unknown processor!
#endif

#ifndef K_ELFDATA_NATIVE
#define K_ELFDATA_NATIVE  ELFDATA2LSB
#endif

K_BOOL K_LoadElf32Segment(const struct Elf32_Phdr *phdr, K_HANDLE image, K_USIZE size)
{
  (void)phdr;
  (void)image;
  (void)size;
  return FALSE;
}

void K_FreeElf32Segment(const struct Elf32_Phdr *phdr, K_HANDLE image, K_USIZE size)
{
  (void)phdr;
  (void)image;
  (void)size;
}

K_BOOL K_LoadElf32Image(K_HANDLE image, K_USIZE size)
{
  struct Elf32_Ehdr *ehdr = image;
  K_U16 index, count;
  if (size < sizeof *ehdr) return FALSE;
  if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) return FALSE;
  if (ehdr->e_ident[EI_DATA] != K_ELFDATA_NATIVE) return FALSE;
  if (ehdr->e_ident[EI_OSABI] || ehdr->e_ident[EI_ABIVERSION]) return FALSE;
  if (ehdr->e_type != ET_EXEC || ehdr->e_machine != K_EM_CUR32) return FALSE;
  if (ehdr->e_version != EV_CURRENT || ehdr->e_ehsize != sizeof *ehdr) return FALSE;
  if (ehdr->e_phentsize != sizeof(struct Elf32_Phdr)) return FALSE;
  count = ehdr->e_phnum;
  for (index = 0; index < count; ++index)
  {
    if (!K_LoadElf32Segment(image + ehdr->e_phoff + ehdr->e_phentsize * index, image, size))
    {
      count = index;
      for (index = 0; index < count; ++index)
      {
        K_FreeElf32Segment(image + ehdr->e_phoff + ehdr->e_phentsize * index, image, size);
      }
      return FALSE;
    }
  }
  return TRUE;
}

#ifdef K_EM_CUR64

K_BOOL K_LoadElf64Segment(const struct Elf64_Phdr *phdr, K_HANDLE image, K_USIZE size)
{
  (void)phdr;
  (void)image;
  (void)size;
  return FALSE;
}

void K_FreeElf64Segment(const struct Elf64_Phdr *phdr, K_HANDLE image, K_USIZE size)
{
  (void)phdr;
  (void)image;
  (void)size;
}

K_BOOL K_LoadElf64Image(K_HANDLE image, K_USIZE size)
{
  struct Elf64_Ehdr *ehdr = image;
  K_U16 index, count;
  if (size < sizeof *ehdr) return FALSE;
  if (ehdr->e_ident[EI_VERSION] != EV_CURRENT) return FALSE;
  if (ehdr->e_ident[EI_DATA] != K_ELFDATA_NATIVE) return FALSE;
  if (ehdr->e_ident[EI_OSABI] || ehdr->e_ident[EI_ABIVERSION]) return FALSE;
  if (ehdr->e_type != ET_EXEC || ehdr->e_machine != K_EM_CUR64) return FALSE;
  if (ehdr->e_version != EV_CURRENT || ehdr->e_ehsize != sizeof *ehdr) return FALSE;
  if (ehdr->e_phentsize != sizeof(struct Elf64_Phdr)) return FALSE;
  count = ehdr->e_phnum;
  for (index = 0; index < count; ++index)
  {
    if (!K_LoadElf64Segment(image + ehdr->e_phoff.Value + ehdr->e_phentsize * index, image, size))
    {
      count = index;
      for (index = 0; index < count; ++index)
      {
        K_FreeElf64Segment(image + ehdr->e_phoff.Value + ehdr->e_phentsize * index, image, size);
      }
      return FALSE;
    }
  }
  return TRUE;
}

#endif

K_BOOL K_LoadTaskImage(K_HANDLE image, K_USIZE size)
{
  const K_U8 *bytes = image;
  if (!image || size < 16) return FALSE;
  if (*bytes == ELFMAG0 && bytes[1] == ELFMAG1 && bytes[2] == ELFMAG2 && bytes[3] == ELFMAG3)
  {
    if (bytes[EI_CLASS] == ELFCLASS32) K_LoadElf32Image(image, size);
#ifdef K_EM_CUR64
    if (bytes[EI_CLASS] == ELFCLASS64) K_LoadElf64Image(image, size);
#endif
  }
  return FALSE;
}

