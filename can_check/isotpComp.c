
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "can_check.h"


Tst_trame isotpComp (Tst_trame trame, Te_isotp_norme *state)
{

  Tst_trame DataBuffer;
  Te_isotp flag_Isotp;


  int L_CF = 0 ;
  int iterator = 0;

  int i = 0;
  int opt;
  int cpt = 0;
 


	switch(*state)
	  		{
	  		case WAIT_S_OR_F_frame: 

	  				if ((trame.data[0] != 0) && (trame.data[0] <= 0x07))
	  				{
	  					memcpy(&DataBuffer.data[0], &trame.data[1], sizeof((trame.taille)-1));
	  				}

	  				else if((trame.data[0] == 0x10) && (trame.taille == 8))
	  				{
	  					flag_Isotp = FIRST_FRAME_OK;
	  					memcpy(&DataBuffer.data[0], &trame.data[2], 6);
	  					
	  					L_CF= (((trame.data[1])-0x06));
	  					printf("%d\n",L_CF);
	  	    			//receive_t_2(DataBuffer);
	  	    			*state = WAIT_FC;
	  	    			
	  				}
	  				else
	  				{
	  					printf("DEBUGGGG\n");
	  					flag_Isotp = FIRST_FRAME_KO;
	  					exit(FIRST_FRAME_KO);
	  				} 
	  				
	  		break;

	  		case WAIT_FC: //floW control

	  				if ((trame.data[0] == 0x30) && (trame.taille == 3))
	  				{
	  					flag_Isotp = FLOW_CONTROL_OK;
	  					printf("OKKK\n");
	  					
	  				}
	  				else 
	  				{
	  					flag_Isotp = FLOW_CONTROL_KO;
	  					exit (FLOW_CONTROL_KO);
	  				}
	  				*state = WAIT_CF;
	  		break;

	  		case WAIT_CF: //Consecutive frame

	  				printf("HEYYYYYYYY\n");
	  				*state = WAIT_S_OR_F_frame;
			break;

	  		default:

	  			  printf("DEFAULT\n");
	  		break;

	  			  



	  		}//fin du switch




  return DataBuffer;
}
