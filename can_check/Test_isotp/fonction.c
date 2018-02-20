#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
//#include "versions.h"
#include "can_check.h"



void receive_t (Tst_trame data)
{
	int i = 0;
	//printf (" %x\n",data.taille);

for (i = 0; i < 6; i++)
		  {
		  	printf ("%02x ", data.data[i]);
		  }
		  printf ("\n");




}
void receive_t_2 (Tst_trame data)
{
int i = 0;
	//printf (" %x\n",data.taille);

for (i = 0; i < 10; i++)
		  {
		  	printf ("%02x ", data.data[i]);
		  }
		  printf ("\n");




}


		if (ligneComplete)
		{
			printf("Debut CF_LC\n");
			memcpy(&DataBuffer.data[6*toto], &trame.data[1],7);
			ligneComplete--;
			toto++;

			if (octetRestant)
				memcpy(&DataBuffer.data[6*toto], &trame.data[1],7);

			else
				*state = WAIT_S_OR_F_frame;


		}

		else if((octetRestant) && (!ligneComplete))
		{
			printf("Debut CF_OR\n");
			memcpy(&DataBuffer.data[6*toto], &trame.data[1], octetRestant);
			*state = WAIT_S_OR_F_frame;
			
		}