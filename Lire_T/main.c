
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
#include "versions.h"




int main (int argc, char *argv[])
{

/*Variable declaration*/
  int s;
  int nbytes, opt;
  char *interface = NULL;
  char *chemin = NULL;
  int protocole = CAN_RAW;
  char s_now[256];
  unsigned char log = 0;
  unsigned char logfrmt = 0;
  unsigned char timestamp = 0;
  unsigned char c = 0;

/*Structure declaration*/
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

  /*Execution option */
  while ((opt = getopt (argc, argv, "i:w:t:z:hv")) != -1)
    {
      switch (opt)
	{
	case 'i':
	  interface = optarg;
	  break;

	case 'w':
	  chemin = optarg;
	  break;

  case 'z':
    protocole = atoi (optarg);
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
	     break; */
  case 'v':
     printf (" \t Version %d.%d.%d \n", MAJOR_V, MINOR_V, BUILD_V);
     printf (" \t Ce fichier a ete compilé le %s a %s \n", __DATE__, __TIME__);
     printf (" \t Le programme a ete execute le %s\n", s_now);
   break; 
  
	case 'h':

    

	default:
	  printf ("\n");
	  utility ();
	  printf ("\n");
	  return 1;
	  break;

	}

    }

  if (interface == NULL)
    {
      printf ("Options are not correct\n");
      printf ("Tape -h to help\n");
      return 1 ; 

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

     } */



  /* Create the socket */
  if ((s = socket (PF_CAN, SOCK_RAW, protocole)) < 0)
    {
      perror ("socket");	// print error
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
  if (bind (s, (struct sockaddr *) &addr, sizeof (addr)) < 0)	// link to a communication point
    {
      perror ("bind");
      return 1;
    }


  int i;
  while (1)
    {
      /*Read frame send from socket */
      nbytes = read (s, &frame, sizeof (frame));
      if (nbytes > 0)
	{
	  FILE *fichier = NULL;
	  fichier = fopen (chemin, "a+");

	  if (chemin != NULL && fichier != NULL)	// writting into a file
	    {

	      /*Reading and writting */
	      fprintf (fichier, " %s  %x  [%d] ", ifr.ifr_name, frame.can_id,
		       frame.can_dlc);
	      for (i = 0; i < frame.can_dlc; i++)
		{

		  fprintf (fichier, " %02x ", frame.data[i]);
		}
	      fprintf (fichier, "\n");

	      fclose (fichier);
	    }

	  else
	    {


	      printf (" %s  %x  [%d] ", ifr.ifr_name, frame.can_id,
		      frame.can_dlc);

        /*c =  frame.data[0];
        frame.data[0] = frame.data[3];
        frame.data[3] = c;

        c =  frame.data[1];
        frame.data[1] = frame.data[2];
        frame.data[2] = c;*/


	      for (i = 0; i < frame.can_dlc; i++)
		{

		  printf (" %02x ", frame.data[i]);	
		}
	      printf ("\n");
	    }

	}


      else
	{
	  error ("Error reading from socket");
	  return 1;
	}


      usleep (1);
    }

  close (s);
  return 0;
}
