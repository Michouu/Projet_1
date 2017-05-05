
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "can_check.h"



int
check (char *line)
{
  int flag = 0;


  if ((strchr (line, '(') == NULL) || ((strchr (line, ')') == NULL)))
    {

      flag = 1;
    }


  return flag;
}
