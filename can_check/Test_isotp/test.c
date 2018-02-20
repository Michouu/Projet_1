#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
//#include "versions.h"
#include "can_check.h"



//Tst_trame trieData(Tst_trame trame,Tst_trame allData,  int incrData);

int
main (int argc, char *argv[])
{
  FILE *fichier = NULL;
  Tst_trame trame;
  Tst_trame DataBuffer;
  Te_isotp flag_Isotp;
  static Te_isotp_norme state; 

  int s_can_id = 0;
  int d_can_id = 0;
  int L_CF = 0 ;
  int iterator = 0;

 
  int i = 0;
  int opt;
  //char state = 'F';
  int cpt = 0;
  state = WAIT_S_OR_F_frame;

  fichier = fopen ("isotp.txt", "r");

  /*for(iterator = 0; iterator < MAX_FRAME; iterator++)
     { */
  while ((opt = getopt (argc, argv, "s:d:")) != -1)
    {
      switch (opt)
	{
	case 's':
	  s_can_id = strtoul(optarg, (char **)NULL, 16);;
	  break;

	case 'd':
	  d_can_id = strtoul(optarg, (char **)NULL, 16);;
	  break;
	}
    }

  printf ("arc = %d, argv[0] = %s, optarg = %s\n", argc, argv[2], optarg);
  //lecture ligne n°iterator = trame

  while (!feof (fichier))	// reading to the end
    {

      fscanf (fichier, " %s  %x  [%hhx]", trame.Nom_interface, &trame.Id,
	      &trame.taille);
      printf (" Frame : %s %x [%x]", trame.Nom_interface, trame.Id,
	      trame.taille);

      for (i = 0; i < trame.taille; i++)	//2nd loop for data
	  {
	  fscanf (fichier, " %hhx ", &trame.data[i]);
	  printf (" %02x ", trame.data[i]);
	  }
      printf ("\n");

      if ((s_can_id == trame.Id) || (d_can_id == trame.Id))
      {
		cpt++;
      }

      else
	   printf ("No ISOTP frame \n");

      if (cpt == 1)
	  {
	  		switch(state)
	  		{
	  		case WAIT_S_OR_F_frame: 

	  				if ((trame.data[0] != 0) && (trame.data[0] <= 0x07))
	  				{
	  					memcpy(&DataBuffer.data[0], &trame.data[1], sizeof((trame.taille)-1));
	  	    			receive_t(DataBuffer);
	  				}

	  				else if((trame.data[0] == 0x10) && (trame.taille == 8))
	  				{
	  					flag_Isotp = FIRST_FRAME_OK;
	  					memcpy(&DataBuffer.data[0], &trame.data[2], 6);
	  					L_CF= (((trame.data[1])-0x06));
	  					printf("%d\n",L_CF);
	  	    			receive_t_2(DataBuffer);
	  	    			state = WAIT_FC;
	  				}
	  				else
	  				{
	  					printf("DEBUGGGG\n");
	  					flag_Isotp = FIRST_FRAME_KO;
	  					return FIRST_FRAME_KO;
	  				} 
	  		break;

	  		case WAIT_FC: //floW control

	  				if ((trame.data[0] == 0x30) && (trame.taille == 3))
	  				{
	  					flag_Isotp = FLOW_CONTROL_OK;
	  					printf("OKKK\n");
	  					state = WAIT_CF;
	  				}
	  				else 
	  				{
	  					flag_Isotp = FLOW_CONTROL_KO;
	  					return FLOW_CONTROL_KO;
	  				}
	  		break;

	  		case WAIT_CF: //Consecutive frame
	  				for (i = 0; i < L_CF ; i ++)
	  				{
	  					
	  				}
	  				printf("HEYYYYYYYY\n");
	  				state = WAIT_S_OR_F_frame;
			break;

	  		default:

	  			  printf("DEFAULT\n");
	  		break;

	  			  



	  		}//fin du switch
	  }

      
	cpt = 0;

    } // fin de lecture de fichier 

    fclose(fichier);
  return 0;
}
