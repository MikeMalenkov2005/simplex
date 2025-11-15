#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define ZERO_PADDING    1
#define LEFT_JUSTIFY    2
#define ALTERNATE_FORM  4
#define FORCE_SIGN      8
#define SPACE_PREFIX    16
#define DOUBLE_MODIFIER 32
#define UPPERCASE       64

static size_t ultoa(unsigned long value, char *buffer, int radix, int uppercase)
{
  const char udig[] = "0123456789ABCDEF";
  const char ldig[] = "0123456789abcdef";
  size_t result = 0;
  char *end, tmp;
  while (value)
  {
    buffer[result++] = (uppercase ? udig : ldig)[value % radix];
    value /= radix;
  }
  end = buffer + result - 1;
  while (buffer < end)
  {
    tmp = *buffer;
    *buffer++ = *end;
    *end-- = tmp;
  }
  return result;
}

int vsnprintf(char *s, size_t n, const char *format, va_list arg)
{
  size_t result, length, i;
  size_t zero_padding_size;
  size_t zero_padding_index;
  const char *start, *string;
  int flags, width, precision, sign;
  char buffer[32], modifier, specifier;
  unsigned long integer;

  result = 0;
  if (format) while (*format)
  {
    while (*format && *format != '%')
    {
      if (result < n) s[result] = *format;
      ++result;
      ++format;
    }
    if (*format)
    {
      modifier = 0;
      specifier = 0;
      start = format++;
      zero_padding_size = 0;
      zero_padding_index = 0;
      flags = width = 0;
      precision = -1;
      string = NULL;
      length = 0;
      while (*format)
      {
        switch (*format++)
        {
        case '0':
          flags |= ZERO_PADDING;
          continue;
        case '-':
          flags |= LEFT_JUSTIFY;
          continue;
        case '+':
          flags |= FORCE_SIGN;
          continue;
        case ' ':
          flags |= SPACE_PREFIX;
          continue;
        case '#':
          flags |= ALTERNATE_FORM;
        case '\'':
          continue;
        }
        --format;
        break;
      }
      if (*format == '*')
      {
        width = va_arg(arg, int);
        ++format;
      }
      else while (*format >= '0' && *format <= '9')
      {
        width = width * 10 + *format++ - '0';
      }
      if (*format == '.')
      {
        ++format;
        precision = 0;
        if (*format == '*')
        {
          precision = va_arg(arg, int);
          ++format;
        }
        else while (*format >= '0' && *format <= '9')
        {
          precision = precision * 10 + *format++ - '0';
        }
      }
      if (*format == 'h' || *format == 'l' || *format == 'L')
      {
        modifier = *format++;
        if (*format == modifier && (*format == 'h' || *format == 'l'))
        {
          flags |= DOUBLE_MODIFIER;
          ++format;
        }
      }
      specifier = *format;
      if (specifier) ++format;
      switch (specifier)
      {
      case 'i':
        specifier = 'd';
        break;
      case 'D':
        specifier = 'd';
        if (modifier == 'l') flags |= DOUBLE_MODIFIER;
        else flags &= ~DOUBLE_MODIFIER;
        modifier = 'l';
        break;
      case 'U':
        specifier = 'u';
        if (modifier == 'l') flags |= DOUBLE_MODIFIER;
        else flags &= ~DOUBLE_MODIFIER;
        modifier = 'l';
        break;
      case 'O':
        specifier = 'o';
        if (modifier == 'l') flags |= DOUBLE_MODIFIER;
        else flags &= ~DOUBLE_MODIFIER;
        modifier = 'l';
        break;
      case 'X':
        flags |= UPPERCASE;
        specifier = 'x';
        break;
      }
      switch (specifier)
      {
      case '%':
        string = format - 1;
        length = 1;
        break;
      case 'c':
        buffer[0] = (char)(unsigned char)va_arg(arg, int);
        string = buffer;
        length = 1;
        break;
      case 's':
        string = va_arg(arg, const char *);
        if (!string) string = "(null)";
        length = strlen(string);
        if (precision >= 0 && (size_t)precision < length) length = precision;
        break;
      case 'p':
      case 'd':
      case 'u':
      case 'x':
      case 'o':
        sign = 0;
        string = buffer;
        if (precision < 0) precision = 1;
        switch (specifier)
        {
        case 'p':
          integer = (unsigned long)va_arg(arg, const void *);
          if (flags & ALTERNATE_FORM) flags |= UPPERCASE;
          flags &= ~ALTERNATE_FORM;
          specifier = 'x';
          break;
        case 'd':
          switch (modifier)
          {
          case 'h':
            integer = (flags & DOUBLE_MODIFIER)
              ? (int)(signed char)va_arg(arg, int)
              : (int)(short)va_arg(arg, int);
            break;
          case 'l':
            integer = va_arg(arg, long);
            break;
          default:
            integer = va_arg(arg, int);
            break;
          }
          sign = integer < 0 ? -1 : 1;
          break;
        default:
          switch (modifier)
          {
          case 'h':
            integer = (flags & DOUBLE_MODIFIER)
              ? (int)(unsigned char)va_arg(arg, unsigned int)
              : (int)(unsigned short)va_arg(arg, unsigned int);
            break;
          case 'l':
            integer = va_arg(arg, unsigned long);
            break;
          default:
            integer = va_arg(arg, unsigned int);
            break;
          }
          break;
        }
        if (sign > 0)
        {
          if (flags & FORCE_SIGN) buffer[length++] = '+';
          else if (flags & SPACE_PREFIX) buffer[length++] = ' ';
        }
        else if (sign < 0)
        {
          buffer[length++] = '-';
          integer = -integer;
        }
        if (specifier == 'x' && (flags & ALTERNATE_FORM) && integer)
        {
          buffer[length++] = '0';
          buffer[length++] = (flags & UPPERCASE) ? 'X' : 'x';
        }
        zero_padding_index = length;
        length = ultoa(integer, buffer + length,
            specifier == 'x' ? 16 : specifier == 'o' ? 8 : 10,
            flags & UPPERCASE);
        if ((size_t)precision > length)
        {
          zero_padding_size = (size_t)precision - length;
        }
        length += zero_padding_index;
        if (specifier == 'o' && (flags & ALTERNATE_FORM) && !zero_padding_size)
        {
          zero_padding_size = 1;
        }
        break;
      default:
        width = flags = 0;
        length = format - start;
        string = start;
        break;
      }
      if (width > 0 && !(flags & LEFT_JUSTIFY))
      {
        if (flags & ZERO_PADDING && (size_t)width > zero_padding_size + length)
        {
          zero_padding_size = (size_t)width - length;
        }
        else while ((size_t)width > zero_padding_size + length)
        {
          if (result < n) s[result] = ' ';
          ++result;
          --width;
        }
      }
      for (i = 0; i < length; ++i)
      {
        if (i == zero_padding_index) while (zero_padding_size--)
        {
          if (result < n) s[result] = '0';
          if (width > 0) --width;
          ++result;
        }
        if (result < n) s[result] = string[i];
        if (width > 0) --width;
        ++result;
      }
      while (width-- > 0)
      {
        if (result < n) s[result] = ' ';
        ++result;
      }
    }
  }
  if (n) s[result < n ? result : n - 1] = 0;
  va_end(arg);
  return (int)result;
}

int vsprintf(char *s, const char *format, va_list arg)
{
  return vsnprintf(s, ~(size_t)0, format, arg);
}

int snprintf(char *s, size_t n, const char *format, ...)
{
  va_list arg;
  va_start(arg, format);
  return vsnprintf(s, n, format, arg);
}

int sprintf(char *s, const char *format, ...)
{
  va_list arg;
  va_start(arg, format);
  return vsprintf(s, format, arg);
}

