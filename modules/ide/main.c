#include "ide.h"

#include <drv/dm.h>
#include <simplex.h>

int main(void)
{
  if (!DM_Register("IDE")) return 1;
  IDE_Init();

  return 0;
}

