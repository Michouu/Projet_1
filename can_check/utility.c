
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonction.h"

int
utility ()
{

	printf("\t ***** HELP *****\n");
	printf("For using this executable, we have to respect some instruction\n\n");
	printf("Usage : ./Comp [options]\n\n");
	printf("\t Options: \n");
	printf("\t    [-f] < file name > \n");
	printf("\t    [-S] : with or without timestamp \n");
	printf("Example:\n");
	printf("./Comp -f file.txt -S\n");

  /*FILE *fichier;



  fichier = fopen ("aides.txt", "r");


  while (!feof (fichier))
    putchar (fgetc (fichier));

  fclose (fichier);

  return 0;*/
}
