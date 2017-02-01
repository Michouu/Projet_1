/*
simple command line tool to send CAN-frames via CAn_RAW sockets
*
All rights reserved.
*
Author: MICHEL Victor <victor.michel@actia.fr>
*
This programm reads frames from a file text and allocate
the differents informations in the structure.
*
Then, these informations are stocked in a socket and send at
the interface due to CAN_RAW.
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>

#include <net/if.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/types.h>
#include <linux/socket.h>
#include <signal.h>

#include "fonctions.h"

#include "versions.h"


int
main (int argc, char *argv[])
{
  Tst_trame var_trames;

  /*Variable declaration */
  int i, j, l;
  int sock; //socket
  int nbytes, opt;
  char *interface = NULL;
  char *file = NULL;
  char s_now[256];
  int tps = 0;
  int protocole = CAN_RAW;
  int timestamp = 0;
  int difference = 0;
  unsigned char c =0;




  /*Structure declaration */
  struct sockaddr_can addr;
  struct can_frame frame;	//frame struct
  struct ifreq ifr;
  struct can_filter rfilter[1];


  time_t t = time (NULL);
  struct tm tm_now = *localtime (&t);
  strftime (s_now, sizeof s_now, " %d/%m/%Y a %H:%M:%S ", &tm_now);



  while ((opt = getopt (argc, argv, "i:f:t:z:vSh")) != -1)	//parsing command line 
    {
      switch (opt)
	{
	case 'i':
	  interface = optarg;	//interface name (can0)
	  break;

	case 'f':
	  file = optarg;	//file name (candump_255)
	  break;

	case 't':
	  tps = atoi (optarg);
	  break;

	case 'z':
	  protocole = atoi (optarg);
	  break;

	case 'S':
	  timestamp = 1;	//whit or whithout Timestamp
	  break;

	
	case 'v':
	  printf (" \t Version %d.%d.%d \n", MAJOR_V, MINOR_V, BUILD_V);
	  printf (" \t This file compiled %s at %s \n", __DATE__, __TIME__);
	  printf (" \t This programm implemented  %s \n", s_now);
	  break;  

	case 'h':

	default:
	  printf ("\n");
	  utility();
	  printf ("\n");
	  return 1;
	  break;

	}

    }

  if ((interface == NULL) || (file == NULL))
    {
      printf ("Options mal renseignees\n");

      return 1;
    }

    
  printf ("\t N_interf = %s, file = %s, time = %d, protocol = %d\n",
	  interface, file, tps, protocole);

  /*Socket creation */
  if ((sock = socket (PF_CAN, SOCK_RAW, protocole)) < 0) //protocole = CAN RAW  
    {
      perror ("socket");
      return 1;
    }

  addr.can_family = AF_CAN;
  /* disable default receive filter on this RAW socket */

  /*interface = argv[1] */
  strcpy (ifr.ifr_name, interface); //interface = CAN0
  /* check if the frame fits into the CAN netdevice */
  if (ioctl (sock, SIOCGIFINDEX, &ifr) < 0)
    {
      perror ("SIOCGIFINDEX");
      return 1;
    }

  addr.can_ifindex = ifr.ifr_ifindex;

  /*link to a communicate point */
  if (bind (sock, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
      perror ("bind");
      return 1;
    }


  FILE *fichier = NULL;

  if (argc > 1)
    {
      //printf ("%d\n", argc);

      fichier = fopen (file, "r");	
    }



  if (fichier != NULL)
    {

      while (!feof (fichier))	// reading until the end 
	{
	  if ((timestamp == 1) && (check (file) == 0))
	    {
	      fscanf (fichier, " (%ld.%d) %s  %x  [%hhx]",
		      &var_trames.sec_tps, &var_trames.usec_tps,
		      var_trames.Nom_interface, &var_trames.Id,
		      &var_trames.taille);

	      
	      	 printf (" frame : (%ld.%d) %s %x [%x]",
		      var_trames.sec_tps, var_trames.usec_tps,
		      var_trames.Nom_interface, var_trames.Id,
		      var_trames.taille);
	      
	     

	    }

	   else if ((timestamp == 0) && (check (file) == 0))
	    {
	      printf ("File check integrity is not correct \n");
	      
		  printf ("Check is timestamp file is correct and argument -S present\n");
	      return 1;


	    }

	  else
	    {
	      /*read and allocate into a variable */
	      fscanf (fichier, " %s  %x [%hhx]", var_trames.Nom_interface,
		      &var_trames.Id, &var_trames.taille);

	      /*size condition */
	      if (var_trames.taille > 8)
		{
		  return 1; 
		}

	      else
		{

	 
		  printf (" frame :" "%s %x [%x]",
			  var_trames.Nom_interface, var_trames.Id,
			  var_trames.taille);
		}
	    }

	  for (i = 0; i < var_trames.taille; i++)	
	    {
	    	 
	      fscanf (fichier, " %hhx ", &var_trames.data[i]);	
	      printf (" %02x ", var_trames.data[i]);	

	    }
	  printf ("\n");

	  /* Allow 29 bits ID */
	  frame.can_id = var_trames.Id;

	  if (var_trames.Id > 0x7FF)	// 0x000007FFU
	    {

	      frame.can_id |= CAN_EFF_FLAG;
	    }

	  /*filling of frame*/
	  frame.can_dlc = var_trames.taille;
	  /*printf (" Frame.id = %02x , Frame.taille = [%x] ",
	     frame.can_id  , frame.can_dlc); */

	  
	  //endianess (var_trames.data[8], c);

	 c =  var_trames.data[0];
	 var_trames.data[0] = var_trames.data[3];
	 var_trames.data[3] = c;

	 c =  var_trames.data[1];
	 var_trames.data[1] = var_trames.data[2];
	 var_trames.data[2] = c;

	

	 for (l = 0; l < var_trames.taille; l++)

	    {

	      frame.data[l]  = var_trames.data[l];	/*filling of frame*/
	      printf ("Frame [%d] : %02x ", l, frame.data[l]);
	    }


	  /* Socket writting  */
	  nbytes = write (sock, &frame, sizeof (frame));
	  if (nbytes < 0)
	    {
	      if (errno = ENOBUFS)	//manage RAM 
		{
		  sched_yield ();	//causes thread

		}
	      else
		{
		  perror ("write");

		}

	    }
	  usleep (tps);		// waiting between every frame
	  printf ("\n\n");

	}


      close (sock);

    }
  else
    {

      printf ("Cannot open the file %s \n", file);
      return 1;

    }
  fclose (fichier);		

  return 0;
}
