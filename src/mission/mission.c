#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "mission.h"
#include "task.h"
#include "../main.h"

void
mission(int speed)
{
    char finished = 0;
    int state = M_START;

	int mission[] = {M_START, M_SQUARE, M_FINISHED}

    do
	{
	    //Mission State Machine
	    switch (state)
		{
		case M_START:
		    state = M_SQUARE;
		case M_FINISHED:
		    task(T_STOP, NULL);
		    finished = 1;
		    break;
		case M_SQUARE:
		    int i;
		    for(i = 0; i < 4; i++)
			{
			    task(T_FORWARD,);
			    task(T_TURN,);
			}
		    state = M_FINISHED;
		    break;
		}
	}
    while(!finished);
}
