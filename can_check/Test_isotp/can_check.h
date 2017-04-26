#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
//#include "versions.h"


/*#define DEBUG

#ifdef DEBUG 

#endif*/

/* Déclaration des variables */
typedef struct
{
  char Nom_interface[5];
  int Id;
  unsigned char taille;
  unsigned char data[8];
  long sec_tps;
  int usec_tps;
  uint64_t compteur;
} Tst_trame;

typedef enum
{
	CAN_CHECK_OK , CAN_CHECK_KO , CAN_CHECK_ERROR_RANDOM
}Te_Result;


int canComp (Tst_trame trame, unsigned long val, char *file);
int check (char *line);
void print_usage(char *prg);

