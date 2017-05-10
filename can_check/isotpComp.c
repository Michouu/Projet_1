
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include "can_check.h"


Tst_trame isotpComp (Tst_trame trame, Te_isotp_norme *state, int *flag)
{

  Tst_trame DataBuffer;
  Te_isotp flag_Isotp;


  double value = 0;
  static double L_CF = 0;
  double result = 0;
  int iterator = 0;

  int i = 0;
  int opt;
  int cpt = 0;
  
 
  static int index = 0;
  DataBuffer.length_data = trame.length_data ;

switch(*state)
	{
	case WAIT_S_OR_F_frame: 

		if ((trame.data[0] != 0) && (trame.data[0] <= 0x07))
		{
		memcpy(&DataBuffer.data[0], &trame.data[1], sizeof((trame.length_data)-1));
		*flag = 1;
		}

		else if(trame.data[0] == 0x10) 
		{
			flag_Isotp = FIRST_FRAME_OK;

			memcpy(&DataBuffer.data[0], &trame.data[2], 6);
			*flag = 0;
			index += 6;

			value = ((trame.data[1])-0x06)/7.0;
			L_CF = ceil(value);

			*state = WAIT_FC;
		}
		else
		{
			flag_Isotp = FIRST_FRAME_KO;
			exit(FIRST_FRAME_KO);
		} 

	break;

	case WAIT_FC: //floW control

		if ((trame.data[0] == 0x30) && (trame.length_data == 3))
		{
			flag_Isotp = FLOW_CONTROL_OK;
			*flag = 0;
			*state = WAIT_CF;
			printf("FC__OKKK\n");
		}
		else 
		{
			flag_Isotp = FLOW_CONTROL_KO;
			exit (FLOW_CONTROL_KO);
		}
		
	break;

	case WAIT_CF: //Consecutive frame

		if(L_CF >= 1)
		{
			memcpy(&DataBuffer.data[index], &trame.data[1], (trame.length_data)-1);
			*flag = 1;
			index += (trame.length_data)-1;
			//memcpy(&DataBuffer.data[(6*nbr)+toto], &trame.data[1], (trame.length_data)-1);
			L_CF --;


			if(L_CF == 0)
			{
				*state = WAIT_S_OR_F_frame;
				index = 0;
			}
		}

	break;

	default:

		printf("DEFAULT\n");
	break;

	}//fin du switch


  return DataBuffer;
		/* code */
	
}
