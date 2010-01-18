#include <stdio.h>
#include <stdlib.h>
#include "mission.h"
#include "task.h"
#include "../main.h"

void
mission(int start_state, int speed)
{

    printf("Mission entered.\n");
    char finished = 0;
    int state = M_START;
    int i;
	
	int term;
    task_parameters para;

    do
	{
	    //Mission State Machine
	    switch (state)
		{
		case M_START:
		    printf("Entered start mission.\n");
		    if(!task(T_FORWARD, speed, ODOMETRY, 1.0)) break;
		    finished = 1;
		    break;
		case M_SQUARE:
		    for(i = 0; i < 4; i++)
			{
			    if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			    if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
			}
		    break;
		case M_DISTANCE_TO_BOX:
		    if(!task(T_FOLLOW_RIGHT, speed, ODOMETRY, 1.5)) break;
		    if(!task(T_FOLLOW, 10, IR_F, 0.3)) break;
		    //printf(current y distance);
		    if(!task(T_TURN, speed, ODOMETRY, M_PI)) break;
		    if(!task(T_FOLLOW, speed, LINE,	/* ARG?! SEE LINE TO RIGHT */)) break;
		    if(!task(T_TURN, speed, ODOMETRY, M_PI)) break;
		    task_id = M_MOVE_OBSTACLE;
		    break;
		case M_MOVE_OBSTACLE:
		    if(!task(T_FOLLOW_STRAIGHT, speed, ODOMETRY, 2.5)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed, IR_F, 0.3)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed / 2, IR_F, 0.15)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed / 2, ODOMETRY, 0.55)) break;
		    if(!task(T_REVERSE, speed, ODOMETRY, 0.95)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
		    if(!task(T_FORWARD, speed / 2, LINE, /* CROSSING BLACK LINE */ );
		    if(!task(T_OCTURN, speed / 2, ODOMETRY, -M_PI / 2)) break;
			if(!task(T_FOLLOW, speed, LINE, /*CROSSING BLACK LINE*/)) break;
			task_id = M_FIND_GHOST_GATE;
		    break;
		case M_FIND_GHOST_GATE:
		    if(!task(T_FOLLOW_RIGHT, speed, LINE, /*CROSSING BLACK LINE*/)) break;
			/*int temp = task(T_FOLLOW, speed/4, ODOMETRY | IR_L, );
			while(!temp && temp != IR_L ) {
				
			}
			if(!temp) {
				break;
			}*/
			break;
		case M_WALL_HUGGING:
		    break;
		case M_WHITE_IS_THE_NEW_BLACK:
		    break;
		case M_HARDCORE_PARKING_ACTION:
		    break;
		}
		
	}
    while(!finished);

    printf("Ending mission.\n");
    if(!task(T_STOP, 0, 0);
}
