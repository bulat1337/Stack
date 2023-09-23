#include <stdio.h>

#include "helpers.h"

void show_bits(int x)
{
    int i = 0;
    for (i = (sizeof(int) * 8) - 1; i >= 0; i--)
    {
       putchar(x & (1 << i) ? '1' : '0');
	   if(i % 4 == 0)
	   {
			putchar(' ');
	   }
    }
    printf("\n");
}
