#include "../../timer.h"
#include "utils.h"

#define EPOCH_YEAR          1970
#define LEAPS_BEFORE_EPOCH  477

#define CMOS_Read(reg)        \
  (K_WritePort8(0x70, reg), K_ReadPort8(0x71))
#define CMOS_Write(reg, val)  \
  (K_WritePort8(0x70, reg), K_WritePort8(0x71, val))

#define BCD2BIN(bcd)  \
  ((((bcd) & 0xF0) >> 1) + (((bcd) & 0xF0) >> 3) + ((bcd) & 0x0F))
#define BIN2BCD(bin)  \
  ((((bin) / 10) << 4) | ((bin) % 10))

/* TODO: Find a way to configure default century */
K_U8 K_DefaultCentury = 20;
K_U8 CMOS_CenturyOffset = 0;

K_SSIZE K_GetRealTime()
{
  int seconds, minutes, hours;
  int day, month, year, century = K_DefaultCentury;
  int leaps, leap, nmi = K_ReadPort8(0x70) & 0x80;
  int bcd = CMOS_Read(nmi | 0x0B) & 4;

  if (CMOS_CenturyOffset)
  {
    century = CMOS_Read(nmi | CMOS_CenturyOffset);
    if (bcd) century = BCD2BIN(century);
  }
  
  year = CMOS_Read(nmi | 0x09);
  month = CMOS_Read(nmi | 0x08);
  day = CMOS_Read(nmi | 0x07);
  hours = CMOS_Read(nmi | 0x04);
  minutes = CMOS_Read(nmi | 0x02);
  seconds = CMOS_Read(nmi | 0x00);

  if (bcd)
  {
    year = BCD2BIN(year);
    month = BCD2BIN(month);
    day = BCD2BIN(day);
    if (hours & 0x80)
    {
      hours &= 0x7F;
      hours = BCD2BIN(hours) + 12;
    }
    else hours = BCD2BIN(hours);
    minutes = BCD2BIN(minutes);
    seconds = BCD2BIN(seconds);
  }

  year += century * 100;
  leap = (!(year % 4) && (year % 100)) || !(year % 400);
  leaps = (year / 4 - year / 100 + year / 400) - LEAPS_BEFORE_EPOCH - leap;

  if (month > 1) day += 31;
  if (month > 2) day += 28 + leap;
  if (month > 3) day += 31;
  if (month > 4) day += 30;
  if (month > 5) day += 31;
  if (month > 6) day += 30;
  if (month > 7) day += 31;
  if (month > 8) day += 31;
  if (month > 9) day += 30;
  if (month > 10) day += 31;
  if (month > 11) day += 30;
  if (hours & 0x80) hours = (hours & 0x7F) + 12;
  
  return ((K_SSIZE)(year - EPOCH_YEAR) * 365 + day - 1 + leaps) * 86400
    + hours * 3600 + minutes * 60 + seconds;
}

K_BOOL K_SetRealTime(K_SSIZE value)
{
  int day, month, year, century, check, leap, nmi = K_ReadPort8(0x70);
  int week_day, hours, minutes, seconds, status = CMOS_Read(nmi | 0x0B);

  if ((seconds = value % 60) < 0) seconds = 60 - seconds;
  value /= 60;
  if ((minutes = value % 60) < 0) minutes = 60 - minutes;
  value /= 60;
  if ((hours = value % 24) < 0) hours = 24 - hours;
  value /= 24;
  if ((week_day = (value + 4) % 7 + 1) < 0) week_day = 7 - week_day;

  year = EPOCH_YEAR;
  check = EPOCH_YEAR % 400;
  leap = 0;
  while (value >= 365 + leap)
  {
    value -= 365 + leap;
    if (++check) check = 0;
    leap = !(value & 3) && value != 100 && value != 200 && value != 300;
    ++year;
  }
  while (value < 0)
  {
    value += 365 + leap;
    if (check--) check = 399;
    leap = !(value & 3) && value != 100 && value != 200 && value != 300;
    --year;
  }
  if (year < 0) return FALSE;
  
  month = 1;
  day = (int)value + 1;
  if (month == 1 && day > 31) ++month, day -= 31;
  if (month == 2 && day > 28 + leap) ++month, day -= 28 + leap;
  if (month == 3 && day > 31) ++month, day -= 31;
  if (month == 4 && day > 30) ++month, day -= 30;
  if (month == 5 && day > 31) ++month, day -= 31;
  if (month == 6 && day > 30) ++month, day -= 30;
  if (month == 7 && day > 31) ++month, day -= 31;
  if (month == 8 && day > 31) ++month, day -= 31;
  if (month == 9 && day > 30) ++month, day -= 30;
  if (month == 10 && day > 31) ++month, day -= 31;
  if (month == 11 && day > 30) ++month, day -= 30;
  century = year / 100;
  year %= 100;

  if (status & 2)
  {
    year = BIN2BCD(year);
    month = BIN2BCD(month);
    day = BIN2BCD(day);
    if (status & 4)
    {
      hours -= 12;
      hours = BIN2BCD(hours) | 0x80;
    }
    else hours = BIN2BCD(hours);
    minutes = BIN2BCD(minutes);
    seconds = BIN2BCD(seconds);
  }

  K_DefaultCentury = century;
  if (CMOS_CenturyOffset)
  {
    if (status & 2) century = BIN2BCD(century);
    CMOS_Write(nmi | CMOS_CenturyOffset, century);
  }
  CMOS_Write(nmi | 0x09, year);
  CMOS_Write(nmi | 0x08, month);
  CMOS_Write(nmi | 0x07, day);
  CMOS_Write(nmi | 0x06, week_day);
  CMOS_Write(nmi | 0x04, hours);
  CMOS_Write(nmi | 0x02, minutes);
  CMOS_Write(nmi | 0x00, seconds);
  return TRUE;
}

