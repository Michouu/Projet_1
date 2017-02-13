#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonctions.h"

//float difference ()

void
utility ()
{

    printf("\t ***** HELP *****\n");
	printf("For using this executable, we have to respect some instruction\n\n");
	printf("Usage : ./Generer [options]\n\n");
	printf("\t Options: \n");
	printf("\t    [-i] < CAN interface > \n");
	printf("\t    [-f] < file name > \n");
	printf("\t    [-t] < gap in microsecond > \n");
	printf("\t    [-z] < ptronbr > (default: CAN_RAW) \n");
	printf("\t    [-S] : with or without timestamp \n");

	printf("Example:\n");
	printf("./Generer -i can0 -f file.txt -t 1000\n");
	printf("./Generer -i can0 -f timestamps.txt -S -z 7 \n");

  

 
}


