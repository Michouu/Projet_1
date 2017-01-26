
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <errno.h>
#include <time.h>

#include <net/if.h>
#include <sys/ioctl.h>

#include <linux/can.h>
#include <linux/can/raw.h>
#include <sys/types.h>
#include "fonction.h"




/*argv un tableau de pointeurs   argc indique le nbr de chaines de cara sur lequel pointe argv*/
int
main (int argc, char *argv[])
{

/*Déclaration de toutes mes variables*/
  int s;
  int nbytes, opt;
  char *interface = NULL;
  char *chemin = NULL;
  char s_now[256];
  unsigned char log = 0;
  unsigned char logfrmt = 0;
  unsigned char timestamp = 0;

/*Déclaration de mes structure*/
  struct sockaddr_can addr;
  struct can_frame frame;
  struct ifreq ifr;
  struct timeval tv, last_tv;

  
  time_t t = time (NULL);
  struct tm tm_now = *localtime (&t);
  strftime (s_now, sizeof s_now, " %d/%m/%Y a %H:%M:%S ", &tm_now);


     /*last_tv.tv_usec = 0;
	 last_tv.tv_sec = 0; */
   //printf("usec = %010ld",last_tv.tv_usec);

  /*Options lors de l'exécution*/
  while ((opt = getopt (argc, argv, "i:w:t:h")) != -1)
	{
	  switch (opt)
	{
	case 'i':
	  interface = optarg;
	  break;

	case 'w':
	  chemin = optarg;
	  break;

	/*case 't':
	  timestamp = optarg[0];
	  if ((timestamp != 'a') && (timestamp != 'A') &&
		  (timestamp != 'd') && (timestamp != 'z'))
		{
		  fprintf (stderr, "%s: unknown timestamp mode '%c' - ignored\n",
			   basename (argv[0]), optarg[0]);
		  timestamp = 0;
		}
	  break;*/

	case 'h':

	default:
	  printf ("\n");
#ifdef DATE
	  printf (" \t Ce fichier a ete compilé le %s a %s \n", __DATE__,
		  __TIME__);
	  printf (" \t Le programme a ete execute le %s", s_now);
#endif
	  utility ();
	  printf ("\n");
	  return 1;
	  break;

	}

	}

	  if (interface == NULL)
	{
	  printf ("Options mal renseignees\n");
	  printf ("Tapez -h pour afficher l'aide\n");
	  return 1;

	}

  /*switch (timestamp)
        {

        case 'a':           //absolute with timestamp 
          printf ("(%010ld.%06ld) ", last_tv.tv_sec, last_tv.tv_usec);
          break;

        case 'A':           //absolute with date 
          {
        struct tm tm;
        char timestring[25];

        tm = *localtime (&tv.tv_usec);
        strftime (timestring, 24, "%Y-%m-%d %H:%M:%S", &tm);
        printf ("(%s.%06lu) ", timestring, last_tv.tv_usec);
          }
          break;

        default:
          break;

        }*/



  /* Create the socket */
  if ((s = socket (PF_CAN, SOCK_RAW, CAN_RAW)) < 0)
	{
	  perror ("socket");    // print error, sert au traitement des erreurs
	  return 1;
	}

  /* Locate the interface you wish to use */
  strcpy (ifr.ifr_name, interface);
  if (ioctl (s, SIOCGIFINDEX, &ifr) < 0)
	{
	  perror ("SIOCGIFINDEX");
	  return 1;
	}

  addr.can_family = AF_CAN;
  addr.can_ifindex = ifr.ifr_ifindex;



  printf ("Read a CAN frame from interface %s \n", ifr.ifr_name);

	/* Select that CAN interface, and bind the socket to it */
  if (bind (s, (struct sockaddr *) &addr, sizeof (addr)) < 0)   // lier à un point de communication défini
	{
	  perror ("bind");
	  return 1;
	}


  int i;
  while (1)
	{
	  /*Lire les trames envoyées par les sockets*/ 	
	  nbytes = read (s, &frame, sizeof (frame));
	  if (nbytes > 0)
	{
	  FILE *fichier = NULL;
	  fichier = fopen (chemin, "a+");

	  if (chemin != NULL && fichier != NULL)
		{

		  /*Lecture + Ecriture des trames dans un fichier texte*/	
		  fprintf (fichier, " %s  %x  [%d] ", ifr.ifr_name, frame.can_id,
			   frame.can_dlc);
		  for (i = 0; i < frame.can_dlc; i++)   //2eme boucle
		{

		  fprintf (fichier, " %02x ", frame.data[i]);   // affic
		}
		  fprintf (fichier, "\n");

		  fclose (fichier);
		}

	  else
		{

               
		  printf (" %s  %x  [%d] ", ifr.ifr_name, frame.can_id,
			  frame.can_dlc);

		  for (i = 0; i < frame.can_dlc; i++)   //2eme boucle
		{

		  printf (" %02x ", frame.data[i]); // affic
		}
		  printf ("\n");
		}

	}


	  else
	{
	  error ("Error reading from socket");
	  return 1;
	}


	  usleep (100000);
	}

  close (s);
  return 0;
}
