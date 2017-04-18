
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "can_check.h"



int
check (char *chaine)
{
  int flag = 0;


  if ((strchr (chaine, '(') == NULL) || ((strchr (chaine, ')') == NULL)))
    {

      flag = 1;
    }


  return flag;
}
