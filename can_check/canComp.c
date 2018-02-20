
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "can_check.h"


int
canComp (  uint64_t counter, unsigned long val, char *file, int id, int incId)
{
  int result = 0;
  int compt = 0;

  if ((val == counter) || (id == incId))
    {
     result = CAN_CHECK_OK;
    }
  
  else 
    {
      result = CAN_CHECK_KO ; 
      fprintf (stderr," Error with exit %d in the file %s line %ld \n",result,file, val+1);
      fprintf (stderr,"-> Frame KO  \n");
      fprintf (stderr," Expected value : 0x%02lx\n Measured value : 0x%02lx\n\n", val,counter);
      printf("CAN_CHECK_KO\n");
     // exit(CAN_CHECK_KO);
    }

return result; 
}
