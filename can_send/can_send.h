#include <time.h>

typedef struct
{
  char Nom_interface[5];
  int Id;
  unsigned char taille;
  unsigned char data[8];
  long sec_tps;
  int usec_tps;
} Tst_trame;

  
int check (char *line);
void print_usage(char *prg);
void endianess (unsigned char d,Tst_trame (*tableau));
void utility ();




