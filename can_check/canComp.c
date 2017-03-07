
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonction.h"


int
canComp (unsigned long trame_compteur, unsigned long val, char *file)
{
  int result = 0;
  unsigned long firstF = 0;


  if (val == trame_compteur)
    {
     result = WELL;
    }
  

  else


    {
      result = ERREUR_INC ; 
      fprintf (stderr," Error with exit %d in the file %s line %ld \n",result,file, val+1);
      fprintf (stderr,"-> Frame KO  \n");
      fprintf (stderr," Expected value : 0x%02lx\n measured value : 0x%02lx\n\n", val,
	       trame_compteur);
    }
return result; 
}
