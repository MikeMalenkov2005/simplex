#include "task.h"
#include "mmu.h"
#include "fpu.h"
#include "utils.h"

static ISR_Frame *TASK_Frame;

void TASK_SetFrame(ISR_Frame *frame)
{
  TASK_Frame = frame;
}

K_HANDLE K_GetTaskIP(K_Task *task)
{
  K_HANDLE result;
  K_Task *current = K_GetCurrentTask();
  if (current != task)
  {
    if (current->PageMap != task->PageMap) K_SetPageMap(task->PageMap);
    result = (K_HANDLE)(K_USIZE)((TASK_Context*)(task->Context + FPU_Size))->Frame.Eip;
    if (current->PageMap != task->PageMap) K_SetPageMap(current->PageMap);
  }
  else result = (K_HANDLE)(K_USIZE)TASK_Frame->Eip;
  return result;
}

void K_SetTaskIP(K_Task *task, K_HANDLE ip)
{
  K_Task *current = K_GetCurrentTask();
  if (current != task)
  {
    if (current->PageMap != task->PageMap) K_SetPageMap(task->PageMap);
    ((TASK_Context*)(task->Context + FPU_Size))->Frame.Eip = (K_U32)(K_USIZE)ip;
    if (current->PageMap != task->PageMap) K_SetPageMap(current->PageMap);
  }
  else TASK_Frame->Eip = (K_U32)(K_USIZE)ip;
}

K_SSIZE K_GetTaskR0(K_Task *task)
{
  K_SSIZE result;
  K_Task *current = K_GetCurrentTask();
  if (current != task)
  {
    if (current->PageMap != task->PageMap) K_SetPageMap(task->PageMap);
    result = (K_SSIZE)((TASK_Context*)(task->Context + FPU_Size))->Frame.Eax;
    if (current->PageMap != task->PageMap) K_SetPageMap(current->PageMap);
  }
  else result = (K_SSIZE)TASK_Frame->Eax;
  return result;
}

void K_SetTaskR0(K_Task *task, K_SSIZE r0)
{
  K_Task *current = K_GetCurrentTask();
  if (current != task)
  {
    if (current->PageMap != task->PageMap) K_SetPageMap(task->PageMap);
    ((TASK_Context*)(task->Context + FPU_Size))->Frame.Eax = (K_U32)r0;
    if (current->PageMap != task->PageMap) K_SetPageMap(current->PageMap);
  }
  else TASK_Frame->Eax = (K_U32)r0;
}

K_HANDLE K_CreateContext(K_USIZE stack, K_U16 flags)
{
  TASK_Context *ctx;
  K_HANDLE context = K_FindLastFreeAddress(FPU_Size + sizeof *ctx);
  if (context && K_AllocatePages(context, FPU_Size + sizeof *ctx, K_PAGE_READABLE | K_PAGE_WRITABLE))
  {
    FPU_Save(context);
    ctx = K_ZeroMemory((context + FPU_Size), sizeof *ctx);
    ctx->StackSize = stack ? stack : K_STACK_SIZE;
    ctx->pStackTop = K_FindLastFreeAddress(ctx->StackSize);
    ctx->Frame.Ebp = ctx->Frame.Esp = (K_U32)(K_USIZE)(ctx->pStackTop + ctx->StackSize);
    ctx->Frame.Eflags = (K_GetCPUFlags() & ~(K_U32)0x3CD5) | ((flags & K_TASK_MODULE) ? 0x3200 : 0x200);
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
  TASK_Context *ctx = (context + FPU_Size);
  FPU_Save(context);
  ctx->Frame = *TASK_Frame;
}

void K_LoadContext(K_HANDLE context)
{
  TASK_Context *ctx = (context + FPU_Size);
  FPU_Load(context);
  *TASK_Frame = ctx->Frame;
}

