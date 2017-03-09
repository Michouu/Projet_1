#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "versions.h"
#include "fonction.h"



void print_usage(char *prg)
{
	fprintf(stderr, "\nUsage: %s [options] <CAN interface>\n",prg);
	fprintf(stderr, "Options: -f <file name>\n");
	fprintf(stderr, "         -S with or without timestamp\n");
	fprintf(stderr, " Example: \n");
	fprintf(stderr, "./Comp -f file.txt \n");
}

int main (int argc, char *argv[])
{
  FILE *fichier = NULL;
  int i = 0, j;
  int compteur = 0;
  int opt;
  Te_Result flag_result = WELL;
  Te_Result result = 0;
  char *file = NULL;
  int timestamp = 0;		//Variable declaration
  int valeur_attendu = 0;	//Variable declaration
  int isotp = 0; 
  int debug = 0;
  static int version = 0;
  char s_now[256];
  time_t t = time (NULL);

  struct tm tm_now = *localtime (&t);
  Tst_trame trame;

  strftime (s_now, sizeof s_now, " %d/%m/%Y a %H:%M:%S ", &tm_now);	//Clock Function


  while ((opt = getopt (argc, argv, "f:SDivh")) != -1)	// parse the commande line
    {
      switch (opt)
	{
	case 'f':
	  file = optarg;
	  break;

	case 'S':
	  timestamp = 1;	//whith or whithout Timestamp
	  break;

	case 'i':
	  isotp = 1;
	  break;

	case 'D':
	  debug = 1;
	  break;

	case 'v':
	  printf (" \t Version %d.%d.%d \n", MAJOR_V, MINOR_V, BUILD_V);
	  printf (" \t This file compiled %s at %s \n", __DATE__, __TIME__);
	  printf (" \t This programm implemented  %s \n", s_now);
	  break;

	case 'h':
		print_usage(argv[0]);
		exit(0);
		break;

	case '?':
		print_usage(argv[0]);
		exit(1);
	  break;

	}

	}

  if ((argv[0] != NULL) && (argc < 2))  
    {
		print_usage(argv[0]);
		return 1;
	}

  if (argc > 1)
    {
		fichier = fopen (file, "r");	// file in option 
		printf ("\t file = %s\n\n", file);

    }


  if (fichier != NULL)
    {

      while (!feof (fichier))	// reading to the end
	  {
	  if ((timestamp == 1) && (check (file, trame) == 0))
	    {
	      fscanf (fichier, " (%ld.%d) %s  %x  [%hhx]", &trame.sec_tps,
		      &trame.usec_tps, trame.Nom_interface,
		      &trame.Id, &trame.taille);

	      if (debug)
	      {
	      printf (" Frame : (%ld.%d) %s %x [%x]",
		      trame.sec_tps, trame.usec_tps,
		      trame.Nom_interface, trame.Id, trame.taille);
	      }
	    }

	  else if((!timestamp) && (check (file, trame) == 0) || (timestamp) && (check (file, trame) == 1))
	    {
	      printf ("File check integrity is not correct \n");
	      print_usage(argv[0]);
	      return 1;
	    }

	  else
	    {
	      fscanf (fichier, " %s  %x  [%hhx]", trame.Nom_interface, &trame.Id, &trame.taille);	// read and allocate in a variable
	      if (debug)
	      	printf (" Frame : %s %x [%x]",trame.Nom_interface, trame.Id, trame.taille);

	      if (trame.taille > 8)	// size condition
		  {
		  printf ("Frame size too big : %hhx \n",trame.taille);
		  return 1;
		  }
	    }

	  for (i = 0; i < trame.taille; i++)	//2nd loop for data
	    {
	      fscanf (fichier, " %hhx ", &trame.data[i]);	// hhx for 8 bits         read and allocate in a variable
	      if (debug)
	      	printf (" %02x ", trame.data[i]);
	    }
	      if (debug)
	  		printf("\n");

	    trame.compteur =
	    trame.data[0] + (trame.data[1] << 8) + (trame.data[2] << 16) +
	    (trame.data[3] << 24) + ((uint64_t)trame.data[4] << 32) + ((uint64_t)trame.data[5] << 40) + ((uint64_t)trame.data[6] << 48) + ((uint64_t)trame.data[7] << 56);
	 
	 if (!isotp)
	 {
	 	result = canComp (trame.compteur, valeur_attendu, file);
	 	valeur_attendu++;
	 }

	 else
	 isotpComp();	

	 if (result == ERROR_INC)
	 	flag_result = -1;

	  }
    }
  else
    {
      printf ("Impossible to open the file: %s \n", file);
      return -1;
    }
  fclose (fichier);		// fermeture du fichier

  if (flag_result == WELL)
  		printf("Return %d\n", WELL);

  else if (flag_result == ERROR_INC)
  		printf("Return %d\n", ERROR_INC);	


  return 0;
}
