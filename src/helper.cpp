#include "helper.h"

#include <cstdio>

void indent_n(int n)
{
  while (n--)
  {
    int rep = 4;
    while (rep--) putc(' ', stdout);
  }
}
