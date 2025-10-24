#ifndef _SYS_DSP_H
#define _SYS_DSP_H

#include <sys/limits.h>
#include <sys/types.h>

#define DSP_READ  1 /* Request to read data             */
#define DSP_WRITE 2 /* Request to write data            */
#define DSP_SEEK  3 /* Request to change position       */
#define DSP_CLOSE 4 /* Request to close a stream        */
#define DSP_OPEN  5 /* Request to open a stream         */
#define DSP_FLUSH 6 /* Request to flush a stream buffer */
#define DSP_REPLY 7 /* Reply with data or error         */

#define DSP_USER  128 /* Start of a user defined range  */

#define DSP_PAYLOAD_SIZE  (K_MESSAGE_SIZE - 4)

struct DSP_Header
{
  K_U16 Stream; /* The Stream/Device ID */
  K_U8  Action; /* The Action/Packet ID */
  K_U8  Length; /* The Data Size  (err) */
};

typedef struct DSP_Header DSP_Header;

struct DSP_Data
{
  DSP_Header Header;
  K_U8 Payload[DSP_PAYLOAD_SIZE];
};

typedef struct DSP_Data DSP_Data;

#define DSP_SEEK_SET  0 /* Offset from start    */
#define DSP_SEEK_CUR  1 /* Offset from position */
#define DSP_SEEK_END  2 /* Offset from end      */

struct DSP_Seek
{
  DSP_Header Header;
  K_U32 SeekMode;
  K_U64S Offset;
};

typedef struct DSP_Seek DSP_Seek;

#define DSP_SUCCESS 0
#define DSP_PARTIAL 254
#define DSP_FAILURE 255

union DSP_Packet
{
  DSP_Header Header;
  DSP_Data Data;
  DSP_Seek Seek;
};

typedef union DSP_Packet DSP_Packet;

#endif

