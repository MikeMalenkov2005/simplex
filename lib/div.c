#include <stdlib.h>

div_t div(int number, int denom)
{
  return (div_t) { number / denom, number % denom };
}

ldiv_t ldiv(long number, long denom)
{
  return (ldiv_t) { number / denom, number % denom };
}

