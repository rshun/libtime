#include <stdio.h>
#include <string.h>
#include "timeutil.h"
#include <stdlib.h>
#include <ctype.h>

static void _func1(const char* dateformat)
{
int ret;

printf("test timeutil_initDateFormat function, parameter=[%s]\n",dateformat);
ret = timeutil_initDateFormat(dateformat);
if (!ret)
{
  printf("function is successful.\n");
}
else
{
  printf("function is failure\n");
}
}


int main(int argc,char* argv[])
{

_func1("YYYYMMDD");
}