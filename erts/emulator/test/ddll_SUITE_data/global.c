#include <stdio.h>
#include "global.h"

extern char extern_symbol;


int
global(void)
{
    return (extern_symbol == MAGIC);
}
