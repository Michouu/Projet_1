#include <stdlib.h>
#include <stdio.h>
#include "versions.h"





/* Déclaration des variables */
typedef struct
{
  char Nom_interface[5];
  int Id;
  unsigned char taille;
  unsigned char data[8];
  long sec_tps;
  int usec_tps;
  unsigned long compteur;
} Tst_trame;


void comparer (unsigned long trame_compteur, unsigned long val);
int check (char *file);
int utility ();
void print_usage(char *prg);
