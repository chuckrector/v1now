#include "xbigdvc.h"
#include "render.h"

void
vcwritelayer(int i)
{
  //   int i;
  //   i=ResolveOperand();

  if (i == 1)
    vcscreen = vcscreen1;
  if (i == 2)
    vcscreen = vcscreen2;
  layervcwrite = i;
  return;
}
