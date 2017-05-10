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
  unsigned char data[4095];
  long sec_tps;
  int usec_tps;
  uint64_t counter;
} Tst_trame;

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

typedef enum 
{
	WAIT_S_OR_F_frame, WAIT_FC, WAIT_CF
}Te_isotp_norme;


int canComp (Tst_trame trame, unsigned long val, char *file);
int check (char *line);
Tst_trame isotpComp (Tst_trame trame, Te_isotp_norme *state, int *flag);
void print_usage(char *prg);

