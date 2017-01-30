#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonctions.h"



int check (char *file)
{
  int  flag=0;
  

  FILE *fichier;
  fichier = fopen (file, "r");
 
  char line[100] = ""; 
  fgets(line,100,fichier);

  if ((strchr(line, '(')==NULL)||((strchr(line, ')')==NULL))) //check to find bracket
  {

     flag=1;
  }
 
 fclose (fichier);    



return flag;
}