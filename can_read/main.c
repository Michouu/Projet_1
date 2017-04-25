
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
#include "can_read.h"
#include "versions.h"


void print_usage(char *prg)
{
  fprintf(stderr, "\nUsage: %s [options] <CAN interface>\n",prg);
  fprintf(stderr, "Options: -i <interface name>\n");
  fprintf(stderr, "         -w < way > path where the user want to create the file\n");
  fprintf(stderr, "         -z <portnbr> (change protocol number default:CAN_RAW)\n");
  fprintf(stderr, " Example: \n");
  fprintf(stderr, "./canRead -i can0 -w /home/documents/file.txt\n");
}


int main (int argc, char *argv[])
{

/*Variable declaration*/
  int s;
  int i;
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
  Te_Erreur erreur = 0;


  time_t t = time (NULL);
  struct tm tm_now = *localtime (&t);
  strftime (s_now, sizeof s_now, " %d/%m/%Y a %H:%M:%S ", &tm_now);

  /*Execution option */
  while ((opt = getopt (argc, argv, "i:w:z:hv")) != -1)
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

  case 'v':
     printf (" \t Version %d.%d.%d \n", MAJOR_V, MINOR_V, BUILD_V);
     printf (" \t Ce fichier a ete compilé le %s a %s \n", __DATE__, __TIME__);
     printf (" \t Le programme a ete execute le %s\n", s_now);
   break; 
  
	case 'h':

    

	default:
	      fprintf(stderr, "Unknown option %c\n", opt);
        print_usage(argv[0]);
        exit(1);
        break;
	}

    }

  if (interface == NULL)
    {
      printf ("Options are not correct\n");
      printf ("Tape -h to help\n");
      return 1 ; 

    }

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
      return CAN_READ_ERROR_INTERFACE;
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

    FILE *fichier = NULL;
    fichier = fopen (chemin, "a+");


  while (1)
    {
      /*Read frame send from socket */
      nbytes = read (s, &frame, sizeof (frame));
      if (nbytes > 0)
	{
	  

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
	  return CAN_READ_KO;
	}

      usleep (1);
    }

  close (s);
  return CAN_READ_OK;
}
