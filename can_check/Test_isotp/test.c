#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
//#include "versions.h"
#include "can_check.h"

//#define MAX_FRAME 55555555

Tst_trame trieData(Tst_trame trame,Tst_trame allData,  int incrData);

int main (int argc, char *argv[])
{
	FILE *fichier = NULL;
	Tst_trame trame;
	int iterator = 0;
	int dataIterator = 0;
	Tst_trame allData;
	int i = 0;

	fichier = fopen ("isotp.txt", "r");

	/*for(iterator = 0; iterator < MAX_FRAME; iterator++)
	{*/
		//lecture ligne n°iterator = trame
		 while (!feof (fichier))	// reading to the end
	  	{
	  		fscanf (fichier, " %s  %x  [%hhx]", trame.Nom_interface, &trame.Id, &trame.taille);
	  		printf (" Frame : %s %x [%x]",trame.Nom_interface, trame.Id, trame.taille);

	  		for (i = 0; i < trame.taille; i++)	//2nd loop for data
	    {
	      fscanf (fichier, " %hhx ", &trame.data[i]);
	      printf (" %02x ", trame.data[i]);
	    }

	    printf("\n");
	    

		trame = trieData(trame, allData, dataIterator);
		

		for(dataIterator = 0; dataIterator < trame.taille; dataIterator++)
		{

			printf("trame.data[%d] = %02x\n",dataIterator, trame.data[dataIterator]);
		}
			if(trame.data[0] != 0)
			{
				/*printf("trame.data(+1) = %02x\n", trame.data[dataIterator+1]);
				printf("trame.data +1 = %02x\n", trame.data[dataIterator]);*/
				if(trame.data[0] == (trame.data[1])+1)
				{
					printf("Tout est bon\n");// OK
				}
				else
				{
					printf("Faux\n");// KO
				}
				
			}
	}
	return 0;
}

Tst_trame trieData(Tst_trame trame,Tst_trame allData,  int incrData)
{


	if(trame.data[0] == 0x10)
	{
		for(incrData = 0; incrData < trame.taille; incrData ++)
		{
			allData.data[incrData] = trame.data[incrData + 1];
			trame.data[incrData] = allData.data[incrData];
			//printf("trames.data[%d] = %02x\n",incrData, trame.data[incrData]);
		}	
		//printf("all = %02x\n", allData.data[incrData]);
	}
	else
	{
		for(incrData = 0; incrData < trame.taille; incrData ++)
		{
			//allData.data[incrData] = trame.data[incrData];
		}
	}

	return trame;
}