#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "can_send.h"



int check (char *ligne)
{
  int  flag=0;
  
  
  if ((strchr(ligne, '(')==NULL)||((strchr(ligne, ')')==NULL))) //check to find bracket
  {

     flag=1;
  }



return flag;
}



