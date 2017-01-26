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

int check (char *file,Tst_trame trames)
{
  int  flag=0;
  /*trames.sec_tps = 0;
  trames.usec_tps = 0;*/

  FILE *fichier;
  fichier = fopen (file, "r");
 
  char line[100] = ""; 
  fgets(line,100,fichier);

  if ((strchr(line, '(')==NULL)||((strchr(line, ')')==NULL)))
  {

     flag=1;
  }
 
 fclose (fichier);    



return flag;
}

  /*printf ("\t \033[1m ***** HELP ***** \n");

     printf (" Usage: ./Programm [options]\n\n");
     printf (" Options:[-i] < CAN interface >\n");
     printf ("         [-f] < file name >\n");
     printf ("         [-t] < gap in microsecond >\n");;
     printf ("         [-z] < portnbr > (default:CAN_RAW)\n\n");
     printf (" Examples:\n");
     printf (" ./Programm -i can0 -f file.txt -t 1000\n\n"); */
