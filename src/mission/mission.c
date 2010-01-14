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
	
    do
	{
	    //Mission State Machine
	    switch (state)
		{
		case M_FINISHED:
		    task(T_STOP, NULL);
		    finished = 1;
		    break;
		}

	    //Stop if keyboard is activated
	    void *arg;
	    ioctl(0, FIONREAD, &arg);
	    if(arg != 0)
		{
		    state = M_FINISHED;
		}
	}
    while(!finished);
}
