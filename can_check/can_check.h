#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "versions.h"


/*#define DEBUG

#ifdef DEBUG 

#endif*/



/* Déclaration des variables */
typedef struct
{
  char interface_name[5];
  int Id;
  unsigned char length_data;
  unsigned char data[8];
  long sec_tps;
  int usec_tps;
  uint64_t counter;
} Tst_trame_CAN;

typedef enum 
{
	WAIT_S_OR_F_frame, WAIT_FC, WAIT_CF
}Te_isotp_norme;

typedef struct 
{
	unsigned char extracting_data[4095];
	Te_isotp_norme state ;
	int index;
	short length_consecutive_frame;
	uint64_t counter;

}Tst_trame_ISOTP;

typedef enum
{
	CAN_CHECK_OK , CAN_CHECK_KO 
}Te_Result;

typedef enum 
{
	FIRST_FRAME_OK , FIRST_FRAME_KO,
	FLOW_CONTROL_OK,FLOW_CONTROL_KO , 
	CONSECUTIVE_FRAME_OK, CONSECUTIVE_FRAME_KO
}Te_isotp;




int canComp (Tst_trame_CAN trame, unsigned long val, char *file);
void IsotpComp (Tst_trame_ISOTP frame, unsigned long val, char *file);
int check (char *line);
int IsotpMode (Tst_trame_CAN *CAN_frame, Tst_trame_ISOTP *ISOTP_frame, int *flag);
void print_usage(char *prg);

