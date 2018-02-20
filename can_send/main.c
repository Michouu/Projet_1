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
#include <sys/time.h>
#include <linux/socket.h>
#include <signal.h>

#include "can_send.h"
#include "versions.h"

#define TAILLE 256


void print_usage(char *prg)
{
	fprintf(stderr, "\nUsage: %s [options] <CAN interface>\n",prg);
	fprintf(stderr, "Options: -f <file name>\n");
	fprintf(stderr, "         -t <gap in microsecond>\n");
	fprintf(stderr, "         -z <portnbr>(change protocol number default:CAN_RAW)\n");
	fprintf(stderr, "         -S with or without timestamp\n");
	fprintf(stderr, "         -D Debug Option\n");
	fprintf(stderr, " Example: \n");
	fprintf(stderr, "./canSend -i can0 -f file.txt -t 1000\n");
	fprintf(stderr, "./canSend -i can0 -f timestamps.txt -S -z 7 \n");
}


int main (int argc, char *argv[])
{
	int diff=0;
	int diff1=0;
  Tst_trame var_trames;
  Tst_trame var_trames_1;
  FILE *fichier = NULL;
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
  int debug = 0;
  int difference = 0;
  unsigned char c =0;
  char line[TAILLE] = "";

  extern int optind, opterr, optopt;


  /*Structure declaration */
  struct timeval tv;
  struct timeval tv_1;
  struct sockaddr_can addr;
  struct can_frame frame;	//frame struct
  struct ifreq ifr;
  struct can_filter rfilter[1];


  time_t t = time (NULL);
  struct tm tm;
  struct tm tm_now = *localtime (&t);
  strftime (s_now, sizeof s_now, " %d/%m/%Y a %H:%M:%S ", &tm_now);



  while ((opt = getopt (argc, argv, "i:f:t:z:DvSh")) != -1)	//parsing command line 
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

	case 'D':
	  debug = 1;
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
		    print_usage(argv[0]);
		    exit(0);
		    break;


	case '?':
		    print_usage(argv[0]);
		    exit(0);
		    break;

	    default:
		    fprintf(stderr, "Unknown option %c\n", opt);
		    print_usage(argv[0]);
		    exit(1);
		    break;

	}
	//printf("optarg = %s argc[%d] argv[%s]\n", optarg, argc,argv[1]);
    }

    if ((argv[0] != NULL) && (argc < 2))  
    {
		print_usage(argv[0]);
		printf("HELLO\n");
		return 1;
	}

    printf ("\t N_interf = %s, file = %s, time = %d, protocol = %d\n",
	  interface, file, tps, protocole);

  /*Socket creation */
  if ((sock = socket (PF_CAN, SOCK_RAW, protocole)) < 0) //protocole = CAN RAW  
    {
      perror ("Error while opening socket");
      return 1;
    }
  addr.can_ifindex = ifr.ifr_ifindex;
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



  /*link to a communicate point */
  if (bind (sock, (struct sockaddr *) &addr, sizeof (addr)) < 0)
    {
      perror ("bind");
      return 1;
    }

  
  fichier = fopen (file, "r");

  fgets(line, TAILLE, fichier);
  rewind (fichier);

  if (fichier != NULL)
    {
    	int counter=0;

      while (!feof (fichier))	// reading until the end 
	{

	  if ((timestamp) && (!(check (line))))
	    {
	      fscanf (fichier, " (%ld.%d) %s  %x  [%hhx]",
		      &var_trames.sec_tps, &var_trames.usec_tps,
		      var_trames.Nom_interface, &var_trames.Id,
		      &var_trames.taille);

	      tv.tv_usec = var_trames.usec_tps;
	      /*tv.tv_sec = var_trames.sec_tps ; 
	      printf("usec = %d\n", tv.tv_usec);
	       printf("sec = %ld\n", tv.tv_sec);*/

	     counter++;

	     if (counter>1)
	     {
	     	//diff=abs(var_trames_1.sec_tps-var_trames.sec_tps);
	     	diff1=abs(tv_1.tv_usec- tv.tv_usec);
	     	//printf("DIFF=%d\n", diff );
	     	printf("DIFF=%d\n", diff1 );
	     }

	     tv_1.tv_usec = var_trames.usec_tps;
	     tv_1.tv_sec = var_trames.sec_tps; 


	      	if (debug)
	      	{
	      	 printf (" frame : (%ld.%d) %s %x [%x]",
		      var_trames.sec_tps, var_trames.usec_tps,
		      var_trames.Nom_interface, var_trames.Id,
		      var_trames.taille);
	      	}

	    }

	   else if ((timestamp) && (check (line)) || (!timestamp) && (!(check (line))))
	    {
	      printf ("File check integrity is not correct \n");
		  print_usage(argv[0]);
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
		   printf ("Frame size too big : %hhx \n",var_trames.taille);
		   return 1;
		}

	      else
		{

	 	  if (debug)
	 	  {
		  printf (" frame :" "%s %x [%x]",
			  var_trames.Nom_interface, var_trames.Id,
			  var_trames.taille);
		  }
		}
	   }

	  for (i = 0; i < var_trames.taille; i++)	
	    {
	    	 
	      fscanf (fichier, " %hhx ", &var_trames.data[i]);	
	      if(debug)
	      printf (" %02x ", var_trames.data[i]);

	    }
	     if(debug)
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

	 for (l = 0; l < var_trames.taille; l++)

	    {
		  frame.data[l]  = var_trames.data[l];	/*filling of frame*/
	      //printf ("Frame [%d] : %02x ", l, frame.data[l]);
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

	    if (timestamp)
	    	usleep(diff1);
	    else
	    	usleep (tps);	
	  	
	    if(debug)
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

  if (ferror(fichier) == 0)
  	printf("CAN_SEND_OK\n");	

  else 
  	printf("CAN_SEND_KO\n");

  return 0;
}
