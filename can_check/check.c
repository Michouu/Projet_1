
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "can_check.h"



int
check (char *line)
{
  int flag = 0;


  if ((strchr (line, '(') == NULL) || ((strchr (line, ')') == NULL)))
    {

      flag = 1;
    }


  return flag;
}

/*int ra(Tst_trame trame)
{
	int compt = 0;
	int flag = 0;

	  if (trame.data[0] == 0x00)
	    	compt ++;

	    if (compt >= 1)
	    	flag = 1;


return flag;				
}
*/