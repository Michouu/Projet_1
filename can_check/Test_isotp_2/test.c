#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
//#include "versions.h"
#include "can_check.h"

//#define MAX_FRAME 55555555

//Tst_trame trieData(Tst_trame trame,Tst_trame allData,  int incrData);

int
main (int argc, char *argv[])
{
  FILE *fichier = NULL;
  Tst_trame trame;
  Te_isotp flag_Isotp = 0;
  int iterator = 0;
  int s_can_id = 0;
  int d_can_id = 0;
  static int singleFrame = 0;
  int dataIterator = 0;
  Tst_trame allData;
  int i = 0;
  int opt;
  int j = 0;
  int cpt = 0;

  fichier = fopen ("isotp.txt", "r");

  /*for(iterator = 0; iterator < MAX_FRAME; iterator++)
     { */
  while ((opt = getopt (argc, argv, "s:d:")) != -1)
    {
      switch (opt)
	{
	case 's':
	  s_can_id = atoi (optarg);
	  printf ("SSSS = %d\n", s_can_id);
	  break;

	case 'd':
	  d_can_id = atoi (optarg);
	  break;
	}
    }

  printf ("arc = %d, argv[0] = %s, optarg = %s\n", argc, argv[2], optarg);
  //lecture ligne n°iterator = trame

  while (!feof (fichier))	// reading to the end
    {
      fscanf (fichier, " %s  %d  [%hhx]", trame.Nom_interface, &trame.Id,
	      &trame.taille);
      printf (" Frame : %s %x [%x]", trame.Nom_interface, trame.Id,
	      trame.taille);

      for (i = 0; i < trame.taille; i++)	//2nd loop for data
	{
	  fscanf (fichier, " %hhx ", &trame.data[i]);
	  printf (" %02x ", trame.data[i]);
	}
      printf ("\n");

      //trame = trieData(trame, allData, dataIterator);
        printf("%d \n", trame.Id);
        printf("%d\n", s_can_id);
        printf("%d\n", d_can_id); 

      if ((s_can_id == trame.Id) || (d_can_id == trame.Id))
		cpt++;


      if (cpt > 0)
	  {
	    if ((trame.data[0] != 0) && (trame.data[0] <= 0x07))
	    {
	      for (dataIterator = 0; dataIterator < (trame.taille) - 1;dataIterator++)
		  {
		  allData.data[dataIterator] = trame.data[dataIterator + 1];
		  printf ("%02x ", allData.data[dataIterator]);
		  }
	      printf ("\n");
	      singleFrame++;
	    }

	    else if ((trame.data[0] > 0x07) && (singleFrame) == 7)
		{
	      flag_Isotp = check_flag_Isotp (trame);
	      printf ("FLAG : %d\n",flag_Isotp );
	     
	      if (flag_Isotp == FIRST_FRAME_OK) // test first frame
		  {
		    for (dataIterator = 0; dataIterator < ((trame.taille) - 2);dataIterator++)
			{
		    allData.data[dataIterator] = trame.data[dataIterator + 2];
			printf ("%02x ", allData.data[dataIterator]);
			}


		  printf ("\n");
		  }


		}
	  else
	    return 1;

	  flag_Isotp = 0;


	  }

      else
	printf ("Id error \n");


    }
  return 0;
}
