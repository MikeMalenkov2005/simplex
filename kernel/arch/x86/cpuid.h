#ifndef _X86_CPUID_H
#define _X86_CPUID_H

#include <sys/types.h>

struct CPUID
{
  K_U32 Ebx;
  K_U32 Edx;
  K_U32 Ecx;
  K_U32 Eax;
};

typedef struct CPUID CPUID;

#define CPUID_INDEX_VENDOR    0
#define CPUID_INDEX_FEATURES  1

#define CPUID_FEATURE_ECX_SSE3        (1 << 0)
#define CPUID_FEATURE_ECX_PCLMUL      (1 << 1)
#define CPUID_FEATURE_ECX_DTES64      (1 << 2)
#define CPUID_FEATURE_ECX_MONITOR     (1 << 3)
#define CPUID_FEATURE_ECX_DS_CPL      (1 << 4)
#define CPUID_FEATURE_ECX_VMX         (1 << 5)
#define CPUID_FEATURE_ECX_SMX         (1 << 6)
#define CPUID_FEATURE_ECX_EST         (1 << 7)
#define CPUID_FEATURE_ECX_TM2         (1 << 8)
#define CPUID_FEATURE_ECX_SSSE3       (1 << 9)
#define CPUID_FEATURE_ECX_CID         (1 << 10)
#define CPUID_FEATURE_ECX_SDBG        (1 << 11)
#define CPUID_FEATURE_ECX_FMA         (1 << 12)
#define CPUID_FEATURE_ECX_CX16        (1 << 13)
#define CPUID_FEATURE_ECX_XTPR        (1 << 14)
#define CPUID_FEATURE_ECX_PDCM        (1 << 15)
#define CPUID_FEATURE_ECX_PCID        (1 << 17)
#define CPUID_FEATURE_ECX_DCA         (1 << 18)
#define CPUID_FEATURE_ECX_SSE4_1      (1 << 19)
#define CPUID_FEATURE_ECX_SSE4_2      (1 << 20)
#define CPUID_FEATURE_ECX_X2APIC      (1 << 21)
#define CPUID_FEATURE_ECX_MOVBE       (1 << 22)
#define CPUID_FEATURE_ECX_POPCNT      (1 << 23)
#define CPUID_FEATURE_ECX_TSC         (1 << 24)
#define CPUID_FEATURE_ECX_AES         (1 << 25)
#define CPUID_FEATURE_ECX_XSAVE       (1 << 26)
#define CPUID_FEATURE_ECX_OSXSAVE     (1 << 27)
#define CPUID_FEATURE_ECX_AVX         (1 << 28)
#define CPUID_FEATURE_ECX_F16C        (1 << 29)
#define CPUID_FEATURE_ECX_RDRAND      (1 << 30)
#define CPUID_FEATURE_ECX_HYPERVISOR  (1 << 31)

#define CPUID_FEATURE_EDX_FPU         (1 << 0)
#define CPUID_FEATURE_EDX_VME         (1 << 1)
#define CPUID_FEATURE_EDX_DE          (1 << 2)
#define CPUID_FEATURE_EDX_PSE         (1 << 3)
#define CPUID_FEATURE_EDX_TSC         (1 << 4)
#define CPUID_FEATURE_EDX_MSR         (1 << 5)
#define CPUID_FEATURE_EDX_PAE         (1 << 6)
#define CPUID_FEATURE_EDX_MCE         (1 << 7)
#define CPUID_FEATURE_EDX_CX8         (1 << 8)
#define CPUID_FEATURE_EDX_APIC        (1 << 9)
#define CPUID_FEATURE_EDX_SEP         (1 << 11)
#define CPUID_FEATURE_EDX_MTRR        (1 << 12)
#define CPUID_FEATURE_EDX_PGE         (1 << 13)
#define CPUID_FEATURE_EDX_MCA         (1 << 14)
#define CPUID_FEATURE_EDX_CMOV        (1 << 15)
#define CPUID_FEATURE_EDX_PAT         (1 << 16)
#define CPUID_FEATURE_EDX_PSE36       (1 << 17)
#define CPUID_FEATURE_EDX_PSN         (1 << 18)
#define CPUID_FEATURE_EDX_CLFLUSH     (1 << 19)
#define CPUID_FEATURE_EDX_DS          (1 << 21)
#define CPUID_FEATURE_EDX_ACPI        (1 << 22)
#define CPUID_FEATURE_EDX_MMX         (1 << 23)
#define CPUID_FEATURE_EDX_FXSR        (1 << 24)
#define CPUID_FEATURE_EDX_SSE         (1 << 25)
#define CPUID_FEATURE_EDX_SSE2        (1 << 26)
#define CPUID_FEATURE_EDX_SS          (1 << 27)
#define CPUID_FEATURE_EDX_HTT         (1 << 28)
#define CPUID_FEATURE_EDX_TM          (1 << 29)
#define CPUID_FEATURE_EDX_IA64        (1 << 30)
#define CPUID_FEATURE_EDX_PBE         (1 << 31)

void CPUID_Init();

void CPUID_Execute(K_U32 index, CPUID *result);

#endif

