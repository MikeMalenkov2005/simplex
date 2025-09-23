#include "loader.h"
#include "memory.h"
#include "utils.h"

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
  K_HANDLE address = (K_HANDLE)(K_USIZE)phdr->p_vaddr;
  K_HANDLE data = image + phdr->p_offset;
  K_U16 flags = K_PAGE_USER_MODE;
  if (phdr->p_type != PT_LOAD || !phdr->p_memsz) return TRUE;
  if (size < phdr->p_offset + phdr->p_filesz) return FALSE;
  if (phdr->p_filesz && !(phdr->p_flags & (PF_R | PF_W | PF_X))) return FALSE;
  if (phdr->p_flags & PF_R) flags |= K_PAGE_READABLE;
  if (phdr->p_flags & PF_W) flags |= K_PAGE_WRITABLE;
  if (phdr->p_flags & PF_X) flags |= K_PAGE_EXECUTABLE;
  if (phdr->p_memsz)
  {
    if (!K_AllocatePages(address, phdr->p_memsz, flags)) return FALSE;
    (void)memcpy(address, data, phdr->p_filesz);
  }
  return TRUE;
}

void K_FreeElf32Segment(const struct Elf32_Phdr *phdr)
{
  if (phdr->p_type == PT_LOAD && phdr->p_memsz) (void)K_FreePages((K_HANDLE)(K_USIZE)phdr->p_vaddr, phdr->p_memsz);
}

K_BOOL K_LoadElf32Image(K_HANDLE image, K_USIZE size, K_HANDLE *entry)
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
  if (size < ehdr->e_phoff + ehdr->e_phnum * ehdr->e_phentsize) return FALSE;
  count = ehdr->e_phnum;
  for (index = 0; index < count; ++index)
  {
    if (!K_LoadElf32Segment(image + ehdr->e_phoff + ehdr->e_phentsize * index, image, size))
    {
      count = index;
      for (index = 0; index < count; ++index)
      {
        K_FreeElf32Segment(image + ehdr->e_phoff + ehdr->e_phentsize * index);
      }
      return FALSE;
    }
  }
  *entry = (K_HANDLE)(K_USIZE)ehdr->e_entry;
  return TRUE;
}

#ifdef K_EM_CUR64

K_BOOL K_LoadElf64Segment(const struct Elf64_Phdr *phdr, K_HANDLE image, K_USIZE size)
{
  K_HANDLE address = (K_HANDLE)(K_USIZE)phdr->p_vaddr.Value;
  K_HANDLE data = image + phdr->p_offset.Value;
  K_U16 flags = K_PAGE_USER_MODE;
  if (size < phdr->p_offset.Value + phdr->p_filesz.Value) return FALSE;
  if (phdr->p_filesz.Value && !(phdr->p_flags & (PF_R | PF_W | PF_X))) return FALSE;
  if (phdr->p_flags & PF_R) flags |= K_PAGE_READABLE;
  if (phdr->p_flags & PF_W) flags |= K_PAGE_WRITABLE;
  if (phdr->p_flags & PF_X) flags |= K_PAGE_EXECUTABLE;
  if (!K_AllocatePages(address, phdr->p_memsz.Value, flags)) return FALSE;
  if (phdr->p_memsz.Value) (void)memcpy(address, data, phdr->p_filesz.Value);
  return TRUE;
}

void K_FreeElf64Segment(const struct Elf64_Phdr *phdr)
{
  if (phdr->p_memsz.Value) K_FreePages((K_HANDLE)phdr->p_vaddr.Value, phdr->p_memsz.Value);
}

K_BOOL K_LoadElf64Image(K_HANDLE image, K_USIZE size, K_HANDLE *entry)
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
  if (size < ehdr->e_phoff.Value + ehdr->e_phnum * ehdr->e_phentsize) return FALSE;
  count = ehdr->e_phnum;
  for (index = 0; index < count; ++index)
  {
    if (!K_LoadElf64Segment(image + ehdr->e_phoff.Value + ehdr->e_phentsize * index, image, size))
    {
      count = index;
      for (index = 0; index < count; ++index)
      {
        K_FreeElf64Segment(image + ehdr->e_phoff.Value + ehdr->e_phentsize * index);
      }
      return FALSE;
    }
  }
  *entry = (K_HANDLE)ehdr->e_entry.Value;
  return TRUE;
}

#endif

K_BOOL K_LoadTaskImage(K_HANDLE image, K_USIZE size, K_HANDLE *entry)
{
  const K_U8 *bytes = image;
  if (!image || size < 16) return FALSE;
  if (bytes[0] == ELFMAG0 && bytes[1] == ELFMAG1 && bytes[2] == ELFMAG2 && bytes[3] == ELFMAG3)
  {
    if (bytes[EI_CLASS] == ELFCLASS32) return K_LoadElf32Image(image, size, entry);
#ifdef K_EM_CUR64
    if (bytes[EI_CLASS] == ELFCLASS64) return K_LoadElf64Image(image, size, entry);
#endif
  }
  return FALSE;
}

