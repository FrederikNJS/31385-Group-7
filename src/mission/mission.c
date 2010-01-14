#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include "mission.h"
#include "task.h"
#include "../main.h"

void
mission(int speed)
{

	printf("Mission entered.\n");
    char finished = 0;
    int state = M_START;
	
    do
	{
	    //Mission State Machine
	    switch (state)
		{
			case M_FINISHED: {
				printf("Entered ending mission.\n");
			    task(T_STOP, NULL);
			    finished = 1;
			    break;
			}
			case M_START: {
				printf("Entered start mission.\n");
				task_parameters para;
				para.speed = speed;
				para.distance = 100.0;
				para.triggers = 0;
				para.triggers |= ODOMETRY;
				task(T_FORWARD, &para);
				state = M_FINISHED;
				break;
			}
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
