void _assert(int value, const char *expression, int line, const char *file)
{
  if (!value)
  {
    (void)expression;
    (void)line;
    (void)file;
  }
}

