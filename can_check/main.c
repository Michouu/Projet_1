#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include "versions.h"
#include "fonction.h"


 //debug(1)

int
main (int argc, char *argv[])
{
  int i = 0, j;
  int compteur = 0;
  int opt;
  char *file = NULL;
  int timestamp = 0;		//Variable declaration
  int valeur_attendu = 0;	//Variable declaration
  static int version = 0;
  char s_now[256];
  time_t t = time (NULL);

  struct tm tm_now = *localtime (&t);
  Tst_trame trame;

  strftime (s_now, sizeof s_now, " %d/%m/%Y a %H:%M:%S ", &tm_now);	//Clock Function


  while ((opt = getopt (argc, argv, "f:SD:vh")) != -1)	// parse the commande line
    {
      switch (opt)
	{
	case 'f':
	  file = optarg;
	  break;

	case 'S':
	  timestamp = 1;	//whith or whithout Timestamp
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
	printf("optarg = %s argc[%d] argv[%s]\n", &optarg[0], argc,argv[1]);
    }

    if ((argv[0] != NULL) && (optind < 1)) {
    	printf("optind = %d argc[%d]\n", optind, argc);
		print_usage(argv[0]);
		return 1;
	}
 /* if (file == NULL)
    {
      printf ("Type -h for help\n");

      return 1;
    }*/

  printf ("\t file = %s\n\n", file);
  FILE *fichier = NULL;

  if (argc > 1)
    {
      fichier = fopen (file, "r");	// file in option 
    }


  if (fichier != NULL)
    {

      while (!feof (fichier))	// reading to the end
	{
	  int f = check (file);
	  if ((timestamp == 1) && (check (file) == 0))
	    {
	      fscanf (fichier, " (%ld.%d) %s  %x  [%hhx]", &trame.sec_tps,
		      &trame.usec_tps, trame.Nom_interface,
		      &trame.Id, &trame.taille);

	      printf (" Frame : (%ld.%d) %s %x [%x]",
		      trame.sec_tps, trame.usec_tps,
		      trame.Nom_interface, trame.Id, trame.taille);
	    }

	  else if ((timestamp == 0) && (check (file) == 0))
	    {
	      printf ("File check integrity is not correct \n");
	      printf ("Check is timestamp file is correct and argument -S present\n");
	      return 1;


	    }


	  else
	    {
	      fscanf (fichier, " %s  %x  [%hhx]", trame.Nom_interface, &trame.Id, &trame.taille);	// read and allocate in a variable

	      if (trame.taille > 8)	// size condition
		{
		  printf ("Frame size too big : %hhx \n",
			  trame.taille);

		  return 1;
		}
	      else
		{

		  printf (" Frame :" "%s %x [%x]",
			  trame.Nom_interface, trame.Id, trame.taille);
		}
	    }

	  for (i = 0; i < trame.taille; i++)	//2nd loop for data
	    {
	      fscanf (fichier, " %hhx ", &trame.data[i]);	// hhx for 8 bits         read and allocate in a variable
	      printf (" %02x ", trame.data[i]);

	    }
	  printf("\n");  

	  trame.compteur =
	    trame.data[0] + (trame.data[1] << 8) + (trame.data[2] << 16) +
	    (trame.data[3] << 24);
	  //printf ("C = 0x%02lx\n", trame.compteur);

	 // comparer (trame.compteur, valeur_attendu);


	  valeur_attendu++;


	}
      printf ("\n");


    }
  else
    {
    	if (argv[0] != NULL)  {
	fprintf(stderr, "Expected argument after options\n");
      	printf("optind = %d argc[%d]\n", optind, argc);
		print_usage(argv[0]);
		return 1;
	}

      printf ("Impossible to open the file: %s \n", file);
      return 1;
    }
  fclose (fichier);		// fermeture du fichier




  return 0;
}
