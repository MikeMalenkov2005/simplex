#include "task.h"
#include "mmu.h"
#include "fpu.h"
#include "utils.h"

static ISR_Frame *TASK_Frame;

void TASK_SetFrame(ISR_Frame *frame)
{
  TASK_Frame = frame;
}

K_HANDLE K_GetTaskIP()
{
  return (K_HANDLE)(K_USIZE)TASK_Frame->Eip;
}

void K_SetTaskIP(K_HANDLE ip)
{
  TASK_Frame->Eip = (K_U32)(K_USIZE)ip;
}

K_HANDLE K_CreateContext(K_USIZE stack, K_U16 flags)
{
  TASK_Context *ctx;
  K_HANDLE context = K_FindLastFreeAddress(FPU_Size + sizeof *ctx);
  if (context && K_AllocatePages(context, FPU_Size + sizeof *ctx, K_PAGE_READABLE | K_PAGE_WRITABLE))
  {
    FPU_Save(context);
    ctx = K_ZeroMemory(context + FPU_Size, sizeof *ctx);
    ctx->StackSize = stack ? stack : K_STACK_SIZE;
    ctx->pStackTop = K_FindLastFreeAddress(ctx->StackSize);
    ctx->Frame.Esp = (K_U32)(K_USIZE)(ctx->pStackTop + ctx->StackSize);
    ctx->Frame.Eflags = (K_GetCPUFlags() & ~(K_U32)0x3CD5) | ((flags & K_TASK_MODULE) ? 0x3020 : 0x20);
    ctx->Frame.Cs = 0x1B;
    ctx->Frame.Ss = 0x23;
    if (!ctx->pStackTop || !K_AllocatePages(ctx->pStackTop, ctx->StackSize, K_PAGE_READABLE | K_PAGE_WRITABLE | K_PAGE_USER_MODE))
    {
      (void)K_FreePages(context, FPU_Size + sizeof *ctx);
      context = NULL;
    }
  }
  else context = NULL;
  return context;
}

void K_DeleteContext(K_HANDLE context)
{
  TASK_Context *ctx = (context + FPU_Size);
  if (context)
  {
    if (ctx->pStackTop) (void)K_FreePages(ctx->pStackTop, ctx->StackSize);
    (void)K_FreePages(context, FPU_Size + sizeof *ctx);
  }
}

void K_SaveContext(K_HANDLE context)
{
  FPU_Save(context);
  *(ISR_Frame*)(context + FPU_Size) = *TASK_Frame;
}

void K_LoadContext(K_HANDLE context)
{
  FPU_Load(context);
  *TASK_Frame = *(ISR_Frame*)(context + FPU_Size);
}

