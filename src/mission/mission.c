#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mission.h"
#include "task.h"
#include "../main.h"
#include "../linesensor/linesensor.h"
#include "../infrared/infrared.h"

void
mission(int start_state, int speed)
{
//    printf("Mission entered at state %d with speed %d\n", start_state, speed);
    char finished = 1;
    int state = start_state;
	
	//TODO: Currently, [2010-01-19, 9:57], these variables are unused.
	//Consider their survival.

    do
	{
		finished = 1;
	    //Mission State Machine
	    switch (state)
		{
		case M_START:
		    if(!task(T_FORWARD, speed, ODOMETRY, 1.0)) break;
		    break;
		case M_SQUARE:
			if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
		    break;
		case M_DISTANCE_TO_BOX:
			system("aplay ~/wav/I\\ Am\\ Bender\\ Please\\ Insert\\ Girder.wav");
		    if(!task(T_FOLLOW_RIGHT, speed, ODOMETRY, 1.5)) break;
		    if(!task(T_FOLLOW_RIGHT, 10, IR_F, 0.15)) break;
			printf("Distance to box: %f\n", (current_odometry.y - 0.2 - read_irsensor_distance(2))*(-1));
			system("aplay ~/wav/I\\'ll\\ have\\ to\\ check\\ my\\ program\\,\\ yep.wav");
		    if(!task(T_TURN, speed/2, ODOMETRY, M_PI)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed, LINE, LINE_DOUBLE)) break;;
		    if(!task(T_FOLLOW_STRAIGHT, speed, ODOMETRY, 0.1)) break;
		    if(!task(T_TURN, speed/2, ODOMETRY, M_PI/2.)) break;
		    state = M_MOVE_OBSTACLE;
			finished = 0;
		    break;
		case M_MOVE_OBSTACLE:
		    if(!task(T_FOLLOW_STRAIGHT, speed, ODOMETRY, 1.0)) break;
			if(!task(T_FOLLOW_STRAIGHT, speed, LINE, LINE_CROSS)) break;
			system("aplay ~/wav/Aww\\ man\\,\\ that\\ crap\\'s\\ heavy.wav");
		    if(!task(T_FOLLOW_STRAIGHT, speed / 2, ODOMETRY, 0.15)) break;
		    if(!task(T_REVERSE, speed, ODOMETRY, 0.95)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
		    if(!task(T_FORWARD, speed / 2, LINE, LINE_CROSS)) break;
		    if(!task(T_OCTURN, speed / 2, ODOMETRY, -2*M_PI / 5)) break;
			if(!task(T_FOLLOW_STRAIGHT, speed/2, LINE, LINE_CROSS)) break;
		    state = M_FIND_GHOST_GATE;
			finished = 0;
		    break;
		case M_FIND_GHOST_GATE:
		    if(!task(T_FORWARD, speed, ODOMETRY, 0.05)) break;
		    if(!task(T_FOLLOW_RIGHT, speed, ODOMETRY, 1.0)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed, LINE, LINE_CROSS)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed, ODOMETRY, 1.5)) break;
			system("aplay ~/wav/Saw\\ a\\ two.wav");
		    if(!task(T_FOLLOW_STRAIGHT, speed, LINE, LINE_CROSS)) break;
/*			int task_status = task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY | IR_L, 1.5, 0.4);
			if(!task_status) break;
			if(task_status == IR_L) { //First rod found, looking for second
				if(!task(T_FORWARD, speed / 4, ODOMETRY, 0.05));
				task_status = task(T_FORWARD, speed/4, ODOMETRY | IR_L, 0.5, 0.4);
				if(!task_status) break;
				if(task_status == IR_L) { //Second rod found, success!
					finished = 0;
					state = M_GO_THROUGH_GHOST_GATE;
					break;
				} else { //Second rod not found
					if(!task(T_REVERSE, speed/4, ODOMETRY, 0.6)) break; //Reverse to before first rod
					task_status = task(T_REVERSE, speed/4, ODOMETRY | IR_L, 0.5, 0.4); //look for second rod before first
					if(!task_status) break;
					if(task_status == IR_L) {//Second rod found behind first
						if(!task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY, 0.45)) break;
						finished = 0;
						state = M_GO_THROUGH_GHOST_GATE;
						break;
					}
				}
			} else if (task_status == ODOMETRY){ //No rods found
				task_status = task(T_REVERSE, speed/4, ODOMETRY | IR_L, 1.5, 0.4);
				if(!task_status) break;
				if(task_status == IR_L) { // First rod found while backing up
					if(!task(T_REVERSE, speed / 4, ODOMETRY, 0.05)) break;
					task_status = task(T_REVERSE, speed/4, ODOMETRY | IR_L, 0.5, 0.4);
					if(!task_status) break;
					if(task_status == IR_L) { //Second rod found while backing up
						if(!task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY, 0.45)) break;
						finished = 0;
						state = M_GO_THROUGH_GHOST_GATE;
						break;
					} else { //Second rod not found while backing up
						if(!task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY, 0.6)) break;
						task_status = task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY | IR_L, 0.5, 0.4);
						if(!task_status) break;
						if(task_status == IR_L) {//Second rod found ahead of first
							finished = 0;
							state = M_GO_THROUGH_GHOST_GATE;
							break;
						}
					}
				}
			}*/
			finished = 0;
			state = M_WALL_HUGGING;
			break;
		case M_GO_THROUGH_GHOST_GATE:
			if(!task(T_TURN, speed/4, ODOMETRY, -M_PI/2)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 0.5)) break;
			if(!task(T_TURN, speed, LINE, BLACK_LINE)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 0.2)) break;
			if(!task(T_TURN, speed, ODOMETRY, -M_PI/2)) break;
		case M_WALL_HUGGING: {
			int task_status;
			task_status = task(T_FOLLOW_STRAIGHT, speed, LINE | IR_L, LINE_CROSS, 0.3);
				if(!task(T_FORWARD, speed, ODOMETRY, 0.1)) break;
				if(!task(T_OCTURN, speed, ODOMETRY, -M_PI/2)) break;
				if(!task(T_FOLLOW_WALL, speed/2, NIR_L, 1.0)) break;
				if(!task(T_FORWARD, speed/3, ODOMETRY, 0.21)) break;
				system("aplay ~/wav/I\\ Don\\'t\\ Need\\ To\\ Drink\\ I\\ Can\\ Quit\\ Anytime\\ I\\ Want.wav");
				if(!task(T_OCTURN, speed/2, ODOMETRY, M_PI/2)) break;
				if(!task(T_REVERSE, speed/2, ODOMETRY, 1.0)) break;
				if(!task(T_OCTURN, speed/2, ODOMETRY, M_PI/2)) break;
				if(!task(T_FOLLOW_WALL, speed/2, LINE, LINE_CROSS)) break;
				if(!task(T_OCTURN, speed/2, ODOMETRY, -M_PI/4)) break;
				if(!task(T_FOLLOW_STRAIGHT, speed, LINE, LINE_CROSS)) break;
			finished = 0;
			state = M_WHITE_IS_THE_NEW_BLACK;
		    break;
		}
		case M_WHITE_IS_THE_NEW_BLACK:
			if(!task(T_FOLLOW_STRAIGHT, speed, ODOMETRY, 0.8)) break;
			if(!task(T_TURN, speed, ODOMETRY, -M_PI/3)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 0.1)) break;
			system("aplay ~/wav/Addictive.wav");
			if(!task(T_FOLLOW_WHITE_STRAIGHT, speed, LINE, LINE_CROSS)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 0.2)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI/2)) break;
			finished = 0;
			state = M_HARDCORE_PARKING_ACTION;
		    break;
		case M_HARDCORE_PARKING_ACTION: {
			if(!task(T_FOLLOW_STRAIGHT, speed/2, IR_F, 0.4)) break;
			if(!task(T_OCTURN, speed/2, ODOMETRY, -M_PI/2)) break;
			if(!task(T_FORWARD, speed/2, ODOMETRY, 0.5)) break;
			if(!task(T_OCTURN, speed/2, ODOMETRY, M_PI/2)) break;
			if(!task(T_FORWARD, speed/2, ODOMETRY, 0.55)) break;
			if(!task(T_ROCTURN, speed/2, ODOMETRY, -M_PI/2)) break;
			if(!task(T_ROCTURN, speed/2, ODOMETRY, M_PI/2)) break;
			if(!task(T_REVERSE, speed/2, ODOMETRY, 0.40)) break;
			if(!task(T_TURN, speed/2, ODOMETRY, M_PI/2)) break;
			if(!task(T_FORWARD, speed/2, LINE, LINE_CROSS)) break;
			if(!task(T_FORWARD, speed/2, ODOMETRY, 0.05)) break;
			if(!task(T_OCTURN, speed/2, ODOMETRY, -2*M_PI/5)) break;
			if(!task(T_FOLLOW_STRAIGHT, speed/2, IR_F, 0.3)) break;
			system("aplay ~/wav/Xmas.wav");
			finished = 1;
		    break;
		}
		case M_TEST:
			if(!task(T_FORWARD, 10, IR_F, 0.15)) break;
		    if(!task(T_TURN, speed, ODOMETRY, M_PI)) break;
		    if(!task(T_FORWARD, speed, LINE, LINE_DOUBLE)) break;
			break;
		}//End of switch.
	}
    while(!finished);

    task(T_STOP, 0, 0);
}
