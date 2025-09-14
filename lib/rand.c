static unsigned long next = 0;

int rand(void)
{
  return ((next = next * 1103515245 + 12345) >> 16) & 0x7FFF;
}

void srand(unsigned int seed)
{
  next = seed;
}

