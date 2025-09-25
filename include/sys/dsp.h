#ifndef _SYS_DSP_H
#define _SYS_DSP_H

#include <sys/limits.h>
#include <types.h>

#define DSP_OPT 0x80
#define DSP_ANS 0x40
#define DSP_CMD 0x03

#define DSP_CFG 0
#define DSP_RX  1
#define DSP_TX  2
#define DSP_ERR 3

struct DSP_Header
{
  K_U16 Index;  /* sequence number  */
  K_U8 Flags;   /* message flags    */
  K_U8 Bytes;   /* byte count       */
};

typedef struct DSP_Header DSP_Header;

struct DSP_Message
{
  DSP_Header Header;
  union
  {
    K_U8 Data[K_MESSAGE_SIZE - 4];
    struct { K_U32 StreamID; K_U8 Data[K_MESSAGE_SIZE - 8]; } Optional;
  };
};

typedef struct DSP_Message DSP_Message;

#endif

