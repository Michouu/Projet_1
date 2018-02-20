#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
//#include "versions.h"
#include "can_check.h"


int check_flag_Isotp (Tst_trame trame)
{
	Te_isotp flag_Isotp = 0;

	switch(trame.data[0])
	{
		case 0x10: 
          if(trame.taille != 8)
		  {	
			flag_Isotp = FIRST_FRAME_KO;
			return FIRST_FRAME_KO;
		  }
		  break;

		case 0x30: 
          if ((trame.data[1] == 0) && (trame.data[2] == 0))
			flag_Isotp = FLOW_CONTROL_OK;
		  else 
		  {
			flag_Isotp = FLOW_CONTROL_KO;
			return FLOW_CONTROL_KO;
		  }	
		  break;

		default:
		  break;
					}

return flag_Isotp;
}