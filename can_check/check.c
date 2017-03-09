
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonction.h"



int
check (char *file, Tst_trame trames)
{
  int flag = 0;


  FILE *fichier;
  fichier = fopen (file, "r");

  char line[100] = "";
  fgets (line, 100, fichier);

  if ((strchr (line, '(') == NULL) || ((strchr (line, ')') == NULL)))
    {

      flag = 1;
    }

  fclose (fichier);



  return flag;
}


/*int check_firstF (Tst_trame trames)
{
  int flag = 0;


  
}*/