
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "can_check.h"


int
canComp (unsigned long trame_compteur, unsigned long val, char *file)
{
  int result = 0;

  if (val == trame_compteur)
    {
     result = CAN_CHECK_OK;
    }
  
  else
    {
      result = CAN_CHECK_KO ; 
      fprintf (stderr," Error with exit %d in the file %s line %ld \n",result,file, val+1);
      fprintf (stderr,"-> Frame KO  \n");
      fprintf (stderr," Expected value : 0x%02lx\n measured value : 0x%02lx\n\n", val,
	       trame_compteur);
    }
return result; 
}
