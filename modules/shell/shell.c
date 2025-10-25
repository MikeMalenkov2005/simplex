#include "shell.h"

#include <simplex.h>
#include <drv/uart.h>

static DSP_Packet out_packet = { 0 };
static DSP_Packet in_packet = { 0 };
static K_U8 in_index = 0;

int SH_Flush()
{
  int result = 0;
  if (out_packet.Header.Length)
  {
    out_packet.Header.Action = DSP_WRITE;
    result = UART_Send(&out_packet);
    if (result != -1) result = UART_Wait(&out_packet);
    if (result != -1)
    {
      result = out_packet.Header.Length = 0;
    }
  }
  return result;
}

int SH_GetChar()
{
  int result = 0;
  if (in_index >= in_packet.Header.Length)
  {
    in_index = 0;
    in_packet.Header.Length = 0;
    while (result != -1 && !in_packet.Header.Length)
    {
      in_packet.Header.Action = DSP_READ;
      in_packet.Header.Length = DSP_PAYLOAD_SIZE;
      result = UART_Send(&in_packet);
      if (result != -1) result = UART_Wait(&in_packet);
      if (result != -1 && in_packet.Header.Length == DSP_FAILURE) result = -1;
    }
    if (result == -1) in_packet.Header.Length = 0;
  }
  if (result != -1) result = in_packet.Data.Payload[in_index++];
  return result;
}

int SH_PutChar(int ch)
{
  out_packet.Data.Payload[out_packet.Header.Length++] = ch;
  if (out_packet.Header.Length == DSP_PAYLOAD_SIZE || ch == '\n')
  {
    if (SH_Flush() == -1)
    {
      --out_packet.Header.Length;
      return -1;
    }
  }
  return 0;
}

int SH_EchoChar()
{
  int result = -1;
  result = SH_GetChar();
  if (result != -1 && (SH_PutChar(result) == -1 || SH_Flush() == -1))
  {
    --in_index;
    result = -1;
  }
  return result;
}

size_t SH_GetString(char *buf, size_t size)
{
  int ch;
  size_t i = 0;
  if (size)
  {
    while ((ch = SH_GetChar()) != -1 && ch != '\r')
    {
      if (ch == '\b')
      {
        if (i) --i;
      }
      else if (i++ < size) buf[i-1] = ch;
    }
    buf[i < size ? i : size - 1] = 0;
  }
  return i < size ? i : size;
}

size_t SH_EchoString(char *buf, size_t size)
{
  int ch;
  size_t i = 0;
  if (size)
  {
    while ((ch = SH_EchoChar()) != -1 && ch != '\r')
    {
      if (ch == '\b')
      {
        if (i)
        {
          --i;
          SH_PutChar(' ');
          SH_PutChar('\b');
        }
        else SH_PutChar(' ');
        SH_Flush();
      }
      else if (i++ < size) buf[i-1] = ch;
    }
    SH_PutChar('\n');
    buf[i < size ? i : size - 1] = 0;
  }
  return i;
}

size_t SH_PutString(const char *str, size_t size)
{
  size_t i = 0;
  while (i < size && SH_PutChar(str[i++]) != -1);
  return i;
}

