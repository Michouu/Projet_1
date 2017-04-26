/*
 * isotpsend.c
 *
 * Copyright (c) 2008 Volkswagen Group Electronic Research
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of Volkswagen nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * Alternatively, provided that this notice is retained in full, this
 * software may be distributed under the terms of the GNU General
 * Public License ("GPL") version 2, in which case the provisions of the
 * GPL apply INSTEAD OF those given above.
 *
 * The provided data structures and external interfaces from this code
 * are not restricted to be used by modules with a GPL compatible license.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
 * DAMAGE.
 *
 * Send feedback to <linux-can@vger.kernel.org>
 *
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h> 
#include <libgen.h>

#include <net/if.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <poll.h>
#include <signal.h>

#include <linux/can.h>
#include <linux/can/isotp.h>
#include <errno.h>

#define DEFAULT_GAP 200 /* ms */
#define NO_CAN_ID 0xFFFFFFFFU
#define BUFSIZE 5000 /* size > 4095 to check socket API internal checks */
#define LENGTH_BYTES_MAX 4095 

typedef enum 
{
	FIXED,INCREMENTED, RANDOM

}Te_Mode;



static volatile int running = 1;

void print_usage(char *prg)
{
	fprintf(stderr, "\nUsage: %s [options] <CAN interface>\n", prg);
	fprintf(stderr, "Options: -s <can_id>  (source can_id. Use 8 digits for extended IDs)\n");
	fprintf(stderr, "         -d <can_id>  (destination can_id. Use 8 digits for extended IDs)\n");
	fprintf(stderr, "         -x <addr>[:<rxaddr>] (extended addressing / opt. separate rxaddr)\n");
	fprintf(stderr, "         -p [tx]:[rx] (set and enable tx/rx padding bytes)\n");
	fprintf(stderr, "         -P <mode>    (check rx padding for (l)ength (c)ontent (a)ll)\n");
	fprintf(stderr, "         -t <time ns> (frame transmit time (N_As) in nanosecs)\n");
	fprintf(stderr, "         -f <time ns> (ignore FC and force local tx stmin value in nanosecs)\n");
	fprintf(stderr, "         -D <len>     (send a fixed PDU with len bytes - no STDIN data)\n");
	fprintf(stderr, "         -T <mode>    (data (payload) generation mode - see below)\n");
	fprintf(stderr, "         -L <mtu>:<tx_dl>:<tx_flags> (link layer options for CAN FD)\n");
	fprintf(stderr, "         -z <portnbr> (change protocol number default:CAN_ISOTP)\n");
	fprintf(stderr, "Options: -g <ms>       (gap in milli seconds "
		"- default: %d ms)\n", DEFAULT_GAP);
	fprintf(stderr, "         -n <count>    (terminate after <count> CAN frames "
		"- default infinite)\n");
	fprintf(stderr, "         -o <timeout>  (poll after write frames"
		" with <timeout> ms)\n");
	fprintf(stderr, "\nCAN IDs and addresses are given and expected in hexadecimal values.\n");
	fprintf(stderr, "The pdu data is expected on STDIN in space separated ASCII hex values.\n");
	fprintf(stderr, "\n");
}

void sigterm(int signo)
{
	running = 0;
}

int main(int argc, char **argv)
{
    
    int s, proto = CAN_ISOTP;
    struct sockaddr_can addr;
    struct ifreq ifr;
    static struct can_isotp_options opts;
    static struct can_isotp_ll_options llopts;
    int opt;
    extern int optind, opterr, optopt;
    __u32 force_tx_stmin = 0;
    unsigned char buf[BUFSIZE] = 0;
    int buflen = 0;
    int datalen = 8;
    Te_Mode data_mode = FIXED;
    Te_Mode dlc_mode = FIXED;
    uint64_t compteur = 0;
    int incdlc = 0;
    int retval = 0;
    double gap = DEFAULT_GAP;
    int count = 0;
    unsigned long polltimeout = 0;
    struct pollfd fds;
    struct timespec ts;
    struct timeval now;

    signal(SIGTERM, sigterm);
    signal(SIGHUP, sigterm);
    signal(SIGINT, sigterm);

    gettimeofday(&now, NULL);
    srandom(now.tv_usec);

    addr.can_addr.tp.tx_id = addr.can_addr.tp.rx_id = NO_CAN_ID;
    printf("version 1.1\n");
    while ((opt = getopt(argc, argv, "s:d:x:p:P:t:f:D:T:L:z:g:n:s:o:?")) != -1) {
	    switch (opt) {
	    case 's':
		    addr.can_addr.tp.tx_id = strtoul(optarg, (char **)NULL, 16);
		    if (strlen(optarg) > 7)
			    addr.can_addr.tp.tx_id |= CAN_EFF_FLAG;
		    break;

	    case 'd':
		    addr.can_addr.tp.rx_id = strtoul(optarg, (char **)NULL, 16);
		    if (strlen(optarg) > 7)
			    addr.can_addr.tp.rx_id |= CAN_EFF_FLAG;
		    break;

	    case 'x':
	    {
		    int elements = sscanf(optarg, "%hhx:%hhx",
					  &opts.ext_address,
					  &opts.rx_ext_address);

		    if (elements == 1)
			    opts.flags |= CAN_ISOTP_EXTEND_ADDR;
		    else if (elements == 2)
			    opts.flags |= (CAN_ISOTP_EXTEND_ADDR | CAN_ISOTP_RX_EXT_ADDR);
		    else {
			    printf("incorrect extended addr values '%s'.\n", optarg);
			    print_usage(basename(argv[0]));
			    exit(0);
		    }
		    break;
	    }

	    case 'p':
	    {
		    int elements = sscanf(optarg, "%hhx:%hhx",
					  &opts.txpad_content,
					  &opts.rxpad_content);

		    if (elements == 1)
			    opts.flags |= CAN_ISOTP_TX_PADDING;
		    else if (elements == 2)
			    opts.flags |= (CAN_ISOTP_TX_PADDING | CAN_ISOTP_RX_PADDING);
		    else if (sscanf(optarg, ":%hhx", &opts.rxpad_content) == 1)
			    opts.flags |= CAN_ISOTP_RX_PADDING;
		    else {
			    printf("incorrect padding values '%s'.\n", optarg);
			    print_usage(basename(argv[0]));
			    exit(0);
		    }
		    break;
	    }

	    case 'P':
		    if (optarg[0] == 'l')
			    opts.flags |= CAN_ISOTP_CHK_PAD_LEN;
		    else if (optarg[0] == 'c')
			    opts.flags |= CAN_ISOTP_CHK_PAD_DATA;
		    else if (optarg[0] == 'a')
			    opts.flags |= (CAN_ISOTP_CHK_PAD_LEN | CAN_ISOTP_CHK_PAD_DATA);
		    else {
			    printf("unknown padding check option '%c'.\n", optarg[0]);
			    print_usage(basename(argv[0]));
			    exit(0);
		    }
		    break;

	    case 't':
		    opts.frame_txtime = strtoul(optarg, (char **)NULL, 10);
		    break;

	    case 'f':
		    opts.flags |= CAN_ISOTP_FORCE_TXSTMIN;
		    force_tx_stmin = strtoul(optarg, (char **)NULL, 10);
		    break;

	    case 'D':
		    if (optarg[0] == 'r')
			{
				dlc_mode = RANDOM;
				datalen = 1;

			}
		    else if (optarg[0] == 'i')
			{
				dlc_mode = INCREMENTED;
				datalen = 1;
			}

		    else 
			{
				dlc_mode = FIXED; 
				datalen = strtoul (optarg, (char **) NULL, 10);
			}

		    if (!datalen || datalen >= BUFSIZE) {
			    print_usage(basename(argv[0]));
			    exit(0);
		    }
		    break;

	    case 'T':
		    if (optarg[0] == 'r')
			{
				data_mode = RANDOM;

			}
		    else if (optarg[0] == 'i')
			{
				data_mode= INCREMENTED;

			}


		    else
			{
			    printf ("unknown link layer options '%s'.\n", optarg);	
			    print_usage (basename (argv[0]));
			    exit (0);
			}

			break;

	    case 'L':
		    if (sscanf(optarg, "%hhu:%hhu:%hhu",
			       &llopts.mtu,
			       &llopts.tx_dl,
			       &llopts.tx_flags) != 3) {
			    printf("unknown link layer options '%s'.\n", optarg);
			    print_usage(basename(argv[0]));
			    exit(0);
		    }
		    break;
		case 'z':
		    proto = atoi(optarg);
		    break;

		case 'g':
			gap = strtod(optarg, NULL);
		break;
		case 'n':
			count = atoi(optarg);
			if (count < 1) {
				print_usage(basename(argv[0]));
				return 1;
		}
		break;
		case 'o':
			polltimeout = strtoul(optarg, NULL, 10);
			break;

	    case '?':
		    print_usage(basename(argv[0]));
		    exit(0);
		    break;

	    default:
		    fprintf(stderr, "Unknown option %c\n", opt);
		    print_usage(basename(argv[0]));
		    exit(1);
		    break;
	    }
    }

    if ((argc - optind != 1) ||
	(addr.can_addr.tp.tx_id == NO_CAN_ID) ||
	(addr.can_addr.tp.rx_id == NO_CAN_ID)) {
	    print_usage(basename(argv[0]));
	    exit(1);
    }

    if ((s = socket(PF_CAN, SOCK_DGRAM, proto)) < 0) {
	perror("socket");
	exit(1);
    }

    setsockopt(s, SOL_CAN_ISOTP, CAN_ISOTP_OPTS, &opts, sizeof(opts));

    if (llopts.tx_dl) {
	if (setsockopt(s, SOL_CAN_ISOTP, CAN_ISOTP_LL_OPTS, &llopts, sizeof(llopts)) < 0) {
	    perror("link layer sockopt");
	    exit(1);
	}
    }

    if (opts.flags & CAN_ISOTP_FORCE_TXSTMIN)
	    setsockopt(s, SOL_CAN_ISOTP, CAN_ISOTP_TX_STMIN, &force_tx_stmin, sizeof(force_tx_stmin));

    addr.can_family = AF_CAN;
    strcpy(ifr.ifr_name, argv[optind]);
    ioctl(s, SIOCGIFINDEX, &ifr);
    addr.can_ifindex = ifr.ifr_ifindex;

    if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
	perror("bind");
	close(s);
	exit(1);
    }

    ts.tv_sec = gap / 1000;
	ts.tv_nsec = (long)(((long long)(gap * 1000000)) % 1000000000ll);

	if (polltimeout) {
		fds.fd = s;
		fds.events = POLLOUT;

	}

	while(running)
	{

    if (!datalen) {
	    while (buflen < BUFSIZE && scanf("%hhx", &buf[buflen]) == 1)
		    buflen++;
    } 

    else
		{
		if (data_mode== INCREMENTED)
		{
			
			*(uint64_t *) buf = compteur;
			compteur++;
		}

		  else
		  {
		    for (buflen = 0; buflen < datalen; buflen++)
		    {

				if (data_mode == RANDOM)
					buf[buflen] = (rand() % 0xFF)+1;

				else if (data_mode == FIXED)
					buf[buflen] = ((buflen % 0xFF) + 1) & 0xFF;
		    }

		  }

		}
	
resend:		if (count && (--count == 0))
			running = 0;

		retval = write(s, buf, datalen);

		if (retval < 0) {
			perror("write");
			if(errno == EADDRINUSE)
				goto resend;

			return retval;
		}

		if (retval != datalen)
			fprintf(stderr, "wrote only %d from %d byte\n", retval, datalen);

		if (polltimeout) {
				/* wait for the write socket (with timeout) */
				if (poll(&fds, 1, polltimeout) < 0) {
					perror("poll");
					return 1;
				}
			}

		if (gap) /* gap == 0 => performance test :-] */
				if (nanosleep(&ts, NULL))
					return 1;

		if (dlc_mode ==INCREMENTED)
		{
		datalen++;
		if (datalen == LENGTH_BYTES_MAX)
			datalen = 1;
		}

		else if (dlc_mode == RANDOM)
		{
		datalen = (rand() % (LENGTH_BYTES_MAX + 1)) + 1;
		}
	}

    /*
     * due to a Kernel internal wait queue the PDU is sent completely
     * before close() returns.
     */
    close(s);

    return 0;
}