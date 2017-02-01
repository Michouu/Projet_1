
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonction.h"


void
comparer (unsigned long trame_compteur, unsigned long val)
{


  if (val == trame_compteur)
    {
      printf ("-> Trame OK \n\n");
    }

  else


    {
      printf ("-> Trame KO  \n");
      fprintf (stderr, " Valeur attendu : 0x%02lx\n Valeur relevée : 0x%02lx\n", val,
	       trame_compteur);
    }


  return;
}
