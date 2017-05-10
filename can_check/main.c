/*****************************************************
Nom ........ : main.c
Role ....... : Compare the frames between them 
*****************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
#include <stdint.h>
#include "versions.h"
#include "can_check.h"

#define TAILLE 256


void print_usage(char *prg)
{
	fprintf(stderr, "\nUsage: %s [options]\n",prg);
	fprintf(stderr, "Options: -f <file name>\n");
	fprintf(stderr, "         -T with or without timestamp\n");
	fprintf(stderr, "         -i isotp frame\n");
	fprintf(stderr, "         -D Debug option\n");
	fprintf(stderr, "         -s source can_id\n");
	fprintf(stderr, "         -d destination can_id\n");
	fprintf(stderr, " Example: \n");
	fprintf(stderr, "./canCheck -f file.txt \n");
}

int main (int argc, char *argv[])
{
 
  FILE *fichier = NULL;
  Tst_trame trame;

  int i = 0;
  int compt = 0;
  int nbr = 1;
  int flag = 0;

  int opt;

  // Isotp Declaration
  int cpt = 0;
  int s_can_id = 0;
  int d_can_id = 0;
  int iterator = 0;
  Te_isotp flag_Isotp;
  Te_isotp_norme state = WAIT_S_OR_F_frame; 
  
  Te_Result result =0;

  
  // argument declaration
  char *file = NULL;
  int timestamp = 0;
  int data_counter = 0;
  int isotp = 0; 
  int debug = 0;
  static int version = 0;

  // check timestamp
  char line[TAILLE] = "";

  char s_now[256];
  time_t t = time (NULL);
  struct tm tm_now = *localtime (&t);
  strftime (s_now, sizeof s_now, " %d/%m/%Y a %H:%M:%S ", &tm_now);	//Clock Function


  while ((opt = getopt (argc, argv, "f:s:d:TDivh")) != -1)	// parse the commande line
    {
      switch (opt)
	{
	case 'f':
	  file = optarg;
	  break;

	case 'T':
	  timestamp = 1;	//whith or whithout Timestamp
	  break;

	case 'i':
	  isotp = 1;
	  break;

	case 'D':
	  debug = 1;
	  break;

	case 's':
	  s_can_id = strtoul(optarg, (char **)NULL, 16);;
	  break;

	case 'd':
	  d_can_id = strtoul(optarg, (char **)NULL, 16);;

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

	} // end of switch

	} //end of parsing argument
	

  if ((argv[0] != NULL) && (argc < 2))  
    {
		print_usage(argv[0]);
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
		      &trame.usec_tps, trame.interface_name,
		      &trame.Id, &trame.length_data);
	      

	      if (debug)
	      {
	      printf (" Frame : (%ld.%d) %s %x [%x]",
		      trame.sec_tps, trame.usec_tps,
		      trame.interface_name, trame.Id, trame.length_data);
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
	      fscanf (fichier, " %s  %x  [%hhx]", trame.interface_name, &trame.Id, &trame.length_data);	// read and allocate in a variable

	      if (debug)
	      	printf (" Frame : %s %x [%x]",trame.interface_name, trame.Id, trame.length_data);

	      if (trame.length_data > 8)	// size condition
		  {
		  printf ("Frame size too big : %hhx \n",trame.length_data);
		  return 1;
		  }
	    }

	  for (i = 0; i < trame.length_data; i++)	//2nd loop for data
	    {
	      fscanf (fichier, " %hhx ", &trame.data[i]);	// hhx for 8 bits         read and allocate in a variable

	      if (debug)
	      	printf (" %02x ", trame.data[i]);
	    }


	      if (debug)
	  		printf("\n");


	    if (isotp)
	    {
			if ((s_can_id == trame.Id) || (d_can_id == trame.Id))
            {
		     cpt++;
            }

			else
			printf ("Not ISOTP frame \n");

			if (cpt == 1)
			trame = isotpComp(trame,&state, &flag);
		
			if(flag)
			{
				for(iterator = 0; iterator < 16; iterator ++)
				{
				printf ("%02x ", trame.data[iterator]);
				}
				printf("\n");
				
			}

	     }

	    cpt = 0;

	    trame.counter =
	    trame.data[0] + (trame.data[1] << 8) + (trame.data[2] << 16) +
	    (trame.data[3] << 24) + ((uint64_t)trame.data[4] << 32) + ((uint64_t)trame.data[5] << 40) + ((uint64_t)trame.data[6] << 48) + ((uint64_t)trame.data[7] << 56);

	    // Call check fonction // 
	 	//result = canComp (trame, data_counter, file);
	 	data_counter++;

	  } //end while

    }//end file

  else
    {
      printf ("Impossible to open the file: %s \n", file);
      return 1;
    }
  fclose (fichier);		// fermeture du fichier


  if ((result == CAN_CHECK_OK) && (ferror(fichier) == 0))
  		printf("CAN_CHECK_OK\n");

  else 
        printf("CAN_CHECK_ERROR_RANDOM\n");	

  return 0;
}
