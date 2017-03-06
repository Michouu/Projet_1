
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonction.h"


void print_usage(char *prg)
{
	fprintf(stderr, "\nUsage: %s [options] <CAN interface>\n",prg);
	fprintf(stderr, "Options: -f <file name>\n");
	fprintf(stderr, "         -S with or without timestamp\n");
	fprintf(stderr, " Example: \n");
	fprintf(stderr, "./Comp -f file.txt \n");


/*int
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

  */
}
