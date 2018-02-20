#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "versions.h"
#include "can_check.h"


void check_debug (Tst_trame_CAN *CAN_frame, Tst_trame_ISOTP *ISOTP_frame, int Timestamp)
{
	if (Timestamp)
	{
	 printf (" Frame : (%ld.%d) %s %x [%x]",
		      CAN_frame->sec_tps, CAN_frame->usec_tps,
		      CAN_frame->interface_name, CAN_frame->Id, CAN_frame->length_data);
	}

	else 
		printf (" Frame : %s %x [%x]",CAN_frame->interface_name, CAN_frame->Id, CAN_frame->length_data);


}