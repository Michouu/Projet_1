#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "fonctions.h"

void endianess (unsigned char d,Tst_trame (*tableau))
{

d =  (*tableau).data[0];
	 (*tableau).data[0] = (*tableau).data[3];
	 (*tableau).data[3] = d;

	 d =  (*tableau).data[1];
	 (*tableau).data[1] = (*tableau).data[2];
	 (*tableau).data[2] = d;

}