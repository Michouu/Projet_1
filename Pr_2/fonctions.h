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

  


void usage ();
int check (char *file,Tst_trame trames);

//int diff_t ( int trame_micro_sec, long trame_sec);
