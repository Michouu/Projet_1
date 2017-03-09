#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "versions.h"


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
	WELL , ERROR_INC = -1
}Te_Result;


int canComp (unsigned long trame_compteur, unsigned long val, char *file);
int check (char *file, Tst_trame trames);
void print_usage(char *prg);
void debug (Tst_trame trames, int i);
