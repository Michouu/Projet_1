#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "versions.h"
#include "can_check.h"
#define TAILLE 256


void print_usage(char *prg)
{
	fprintf(stderr, "\nUsage: %s [options] <CAN interface>\n",prg);
	fprintf(stderr, "Options: -f <file name>\n");
	fprintf(stderr, "         -S with or without timestamp\n");
	fprintf(stderr, "         -i isotp frame\n");
	fprintf(stderr, "         -D Debug option\n");
	fprintf(stderr, " Example: \n");
	fprintf(stderr, "./Comp -f file.txt \n");
}

int main (int argc, char *argv[])
{
  FILE *fichier = NULL;
  int i = 0, j;
  int compt = 0;
  int opt;
   int flag = 0;
  Te_Result flag_result = 0;
  Te_Result result =0;
  char *file = NULL;
  int timestamp = 0;		//Variable declaration
  int valeur_attendu = 0;	//Variable declaration
  int isotp = 0; 
  int debug = 0;
  static int version = 0;
  char s_now[256];
  char line[TAILLE] = "";
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
		    exit(0);
		    break;	

	  default:
		    fprintf(stderr, "Unknown option %c\n", opt);
		    print_usage(argv[0]);
		    exit(1);
		    break;

	}
printf ("argc = %d\n", argc);
	}
	

  if ((argv[0] != NULL) && (argc < 2))  
    {
		print_usage(argv[0]);
		printf("HELLO\n");
		return 1;
	}


		fichier = fopen (file, "r");	// file in option 
		printf ("\t file = %s\n\n", file);

		fgets(line, TAILLE, fichier);
		rewind(fichier);	

  if (fichier != NULL)
    {
    	
      while (!feof (fichier))	// reading to the end
	  {
	  if ((timestamp) && (!(check (line))))
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

	  else if ((timestamp) && (check (line)) || (!timestamp) && (!(check (line))))
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
		  printf("\n");
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

	    if (trame.data[0] == 0x00)
	    	compt ++;

	      if (debug)
	  		printf("\n");


	    trame.compteur =
	    trame.data[0] + (trame.data[1] << 8) + (trame.data[2] << 16) +
	    (trame.data[3] << 24) + ((uint64_t)trame.data[4] << 32) + ((uint64_t)trame.data[5] << 40) + ((uint64_t)trame.data[6] << 48) + ((uint64_t)trame.data[7] << 56);
	
	 if ((!isotp) && (compt))
	 {

	 	result = canComp (trame, valeur_attendu, file);
	 	valeur_attendu++;
	 }

	 else
	 	flag_result = CAN_CHECK_ERROR_RANDOM;


	 if (isotp)
	 isotpComp();	

	 if (result == CAN_CHECK_KO)
	 	flag_result = result;

	  }
    }
  else
    {
      printf ("Impossible to open the file: %s \n", file);
      return 1;
    }
  fclose (fichier);		// fermeture du fichier

  if ((flag_result == CAN_CHECK_OK) && (ferror(fichier) == 0))
  		printf("CAN_CHECK_OK\n");

  else if (flag_result == CAN_CHECK_KO)
  		printf("CAN_CHECK_KO\n");	

  else 
        printf("CAN_CHECK_ERROR_RANDOM\n");	

  return 0;
}
