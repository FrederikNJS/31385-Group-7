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

    task_parameters para;

    do
	{
	    //Mission State Machine
	    switch (state)
		{
		case M_START:
		    printf("Entered start mission.\n");
		    para.speed = speed;
		    para.distance = 100.0;
		    para.triggers = ODOMETRY;
		    task(T_FORWARD, &para);
		    state = M_FINISHED;
		    break;
		case M_SQUARE:
		    int i;
		    for(i = 0; i < 4; i++)
			{
			    //task(T_FORWARD,);
			    //task(T_TURN,);
			}
		    break;
		}
	}
    while(!finished);

	printf("ending mission.\n");
    task(T_STOP, NULL);
}
