/*****************************************************
Nom ........ : main.c
Role ....... : Compare the frames between them 
*****************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "versions.h"
#include "can_check.h"

#define TAILLE 256
#define BYTES_MAX 4095



void print_usage(char *prg)
{
	fprintf(stderr, "\nUsage: %s [options]\n",prg);
	fprintf(stderr, "Options: -f <file name>\n");
	fprintf(stderr, "         -T with or without timestamp\n");
	fprintf(stderr, "         -i isotp frame\n");
	fprintf(stderr, "         -D Debug option\n");
	fprintf(stderr, "         -s source can_id\n");
	fprintf(stderr, "         -d destination can_id\n");
	fprintf(stderr, ".........-I Incrementation ID"); 
	fprintf(stderr, " Example: \n");
	fprintf(stderr, "./canCheck -f file.txt \n");
}

int main (int argc, char *argv[])
{
 
  FILE *fichier = NULL;

  Tst_trame_CAN st_CAN_frame;
  Tst_trame_ISOTP st_ISOTP_frame;
  Te_isotp Isotp_flag =0;

  st_ISOTP_frame.index = 0;
  st_ISOTP_frame.state = WAIT_S_OR_F_frame;
  st_ISOTP_frame.length_consecutive_frame = 0;

  int i = 0;
  int flag_Display = 0;
  uint64_t counter = 0;

  int opt;

  // Isotp Declaration
  int control_ID = 0;
  int s_can_id = 0;
  int d_can_id = 0;
  int iterator = 0;
  
  Te_Result result =0;

  
  // argument declaration
  char *file = NULL;
  int timestamp = 0;
  int CAN_data_counter = 0;
  int Isotp_data_counter = 0;
  int isotp = 0; 
  int incrementationId = 0;
  int check_data_counter = 0;
  int debug = 0;
  int display_Data = 0; 
  static int version = 0;

  // check timestamp
  char line[TAILLE] = "";

  char s_now[256];
  time_t t = time (NULL);
  struct tm tm_now = *localtime (&t);
  strftime (s_now, sizeof s_now, " %d/%m/%Y a %H:%M:%S ", &tm_now);	//Clock Function


  while ((opt = getopt (argc, argv, "f:s:d:TDivhI")) != -1)	// parse the commande line
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
	  d_can_id = strtoul(optarg, (char **)NULL, 16);
	  break;

	case 'I':
	  incrementationId = 1; 
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

	      fscanf (fichier, " (%ld.%d) %s  %x  [%hhx]", &st_CAN_frame.sec_tps,
		      &st_CAN_frame.usec_tps, st_CAN_frame.interface_name,
		      &st_CAN_frame.Id, &st_CAN_frame.length_data);
	      

	      if (debug)
	      {
	      	check_debug (&st_CAN_frame, &st_ISOTP_frame, timestamp);
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
	      fscanf (fichier, " %s  %x  [%hhx]", st_CAN_frame.interface_name, &st_CAN_frame.Id, &st_CAN_frame.length_data);	// read and allocate in a variable

	      if (debug)
	      	check_debug (&st_CAN_frame, &st_ISOTP_frame, timestamp);

	      if (st_CAN_frame.length_data > 8)	// size condition
		  {
		  printf ("Frame size too big : %hhx \n",st_CAN_frame.length_data);
		  return 1;
		  }
	    }

	  for (i = 0; i < st_CAN_frame.length_data; i++)	//2nd loop for data
	    {
	      fscanf (fichier, " %hhx ", &st_CAN_frame.data[i]);	// hhx for 8 bits         read and allocate in a variable

	      if (debug)
	      	printf (" %02x ", st_CAN_frame.data[i]);
	    }
	      if (debug)
	  		printf("\n");


	  if (isotp)
	  {
			if ((s_can_id == st_CAN_frame.Id) || (d_can_id == st_CAN_frame.Id))
            {
		     control_ID++;
            }

			else
			printf ("Not ISOTP frame \n");

		  if (control_ID == 1){
		  	Isotp_flag = IsotpMode(&st_CAN_frame, &st_ISOTP_frame, &flag_Display);
		  	display_Data ++;
		  }
			
		  if((flag_Display) && (debug))
		  {
				for(iterator = 0; iterator < display_Data; iterator ++)
				{
				printf ("%02x ", st_ISOTP_frame.extracting_data[iterator]);
				}
				printf("\n");
				
		  }

		  if (st_ISOTP_frame.length_consecutive_frame == 0)
		  {
			st_ISOTP_frame.state = WAIT_S_OR_F_frame;
			st_ISOTP_frame.index = 0;
		  }
			
	  }

	    control_ID = 0;

	    if((isotp) && (st_ISOTP_frame.index == 0))
	    {
	    counter =
	    st_ISOTP_frame.extracting_data[0] + (st_ISOTP_frame.extracting_data[1] << 8) + (st_ISOTP_frame.extracting_data[2] << 16) +
	    (st_ISOTP_frame.extracting_data[3] << 24) + ((uint64_t)st_ISOTP_frame.extracting_data[4] << 32) + ((uint64_t)st_ISOTP_frame.extracting_data[5] << 40) + ((uint64_t)st_ISOTP_frame.extracting_data[6] << 48) + ((uint64_t)st_ISOTP_frame.extracting_data[7] << 56);
		
		 IsotpComp (counter, Isotp_data_counter, file);
		 Isotp_data_counter++;
	    }

	 	else if(isotp == 0)
	 	{
	    counter =
	    st_CAN_frame.data[0] + (st_CAN_frame.data[1] << 8) + (st_CAN_frame.data[2] << 16) +
	    (st_CAN_frame.data[3] << 24) + ((uint64_t)st_CAN_frame.data[4] << 32) + ((uint64_t)st_CAN_frame.data[5] << 40) + ((uint64_t)st_CAN_frame.data[6] << 48) + ((uint64_t)st_CAN_frame.data[7] << 56);
	 		
	 	if (incrementationId == 1)
	 	{
	 		//iteratorId = st_CAN_frame.Id; 
	 		canComp (counter, CAN_data_counter, file,st_CAN_frame.Id, check_data_counter);
	 		CAN_data_counter++;
	 		check_data_counter ++; 
	 	}
	 	
	 	else
	 		/*canComp (counter, CAN_data_counter, file);*/
	 		CAN_data_counter++;
	 	}

	 	
	 	

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
