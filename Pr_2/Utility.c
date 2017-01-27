#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonctions.h"

//float difference ()

int
utility ()
{

  FILE *fichier;



  fichier = fopen ("aides.txt", "r");


  while (!feof (fichier))
    putchar (fgetc (fichier));

  fclose (fichier);

  return 0;
}


