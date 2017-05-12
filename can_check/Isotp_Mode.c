
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>
#include <time.h>
#include "can_check.h"
#define BYTES_MAX 4095


int IsotpMode (Tst_trame_CAN *CAN_frame, Tst_trame_ISOTP *ISOTP_frame, int *flag)
{

  Te_isotp flag_Isotp;


   int value_byte_remaining = 0;


switch(ISOTP_frame->state)
	{
	case WAIT_S_OR_F_frame: 

		if ((CAN_frame->data[0] != 0) && (CAN_frame->data[0] <= 0x07))
		{
		memcpy(&ISOTP_frame->extracting_data[0], &CAN_frame->data[1], sizeof((CAN_frame->length_data)-1));
		*flag = 1;
		}

		else if(CAN_frame->data[0] == 0x10) 
		{
			flag_Isotp = FIRST_FRAME_OK;

			memcpy(&ISOTP_frame->extracting_data[0], &CAN_frame->data[2], 6);
			*flag = 0;
			ISOTP_frame->index += 6;

			value_byte_remaining = ((CAN_frame->data[1])-0x06)%7;
			ISOTP_frame->length_consecutive_frame = ((CAN_frame->data[1])-0x06)/7;

			ISOTP_frame->length_consecutive_frame += (value_byte_remaining !=0) ? 1 : 0;
			ISOTP_frame->state = WAIT_FC;
		}
		else
		{
			flag_Isotp = FIRST_FRAME_KO;
			exit(FIRST_FRAME_KO);
		} 

	break;

	case WAIT_FC: //floW control

		if ((CAN_frame->data[0] == 0x30) && (CAN_frame->length_data == 3))
		{
			flag_Isotp = FLOW_CONTROL_OK;
			*flag = 0;
			ISOTP_frame->state = WAIT_CF;
		}
		else 
		{
			flag_Isotp = FLOW_CONTROL_KO;
			exit (FLOW_CONTROL_KO);
		}
		
	break;

	case WAIT_CF: //Consecutive frame

		//printf("ISOTP_frame->length_consecutive_frame : %d\n",ISOTP_frame->length_consecutive_frame);

		if(ISOTP_frame->length_consecutive_frame >= 1)
		{
			memcpy(&ISOTP_frame->extracting_data[ISOTP_frame->index], &CAN_frame->data[1], (CAN_frame->length_data)-1);
			*flag = 1;

			ISOTP_frame->index += (CAN_frame->length_data)-1;
			ISOTP_frame->length_consecutive_frame --;
		}

	break;

	default:

		printf("DEFAULT\n");
	break;

	}//fin du switch


  return flag_Isotp;

}
