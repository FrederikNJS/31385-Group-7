#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "mission.h"
#include "task.h"
#include "../main.h"
#include "../linesensor/linesensor.h"

void
mission(int start_state, int speed)
{
    printf("Mission entered at state %d with speed %d\n", start_state, speed);
    char finished = 1;
    int state = start_state;
    int i;
	
	//TODO: Currently, [2010-01-19, 9:57], these variables are unused.
	//Consider their survival.
	int term;

    do
	{
		finished = 1;
	    //Mission State Machine
	    switch (state)
		{
		case M_START:
		    printf("Entered start mission.\n");
		    if(!task(T_FORWARD, speed, ODOMETRY, 1.0)) break;
		    break;
		case M_SQUARE:
			printf("In the square1!\n");
			if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			printf("In the square2!\n");
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
			printf("In the square3!\n");
			if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 3.0)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
		    break;
		case M_DISTANCE_TO_BOX:
			printf("In the box\n");
		    if(!task(T_FOLLOW_RIGHT, speed, ODOMETRY, 1.5)) break;
		    if(!task(T_FOLLOW_STRAIGHT, 10, IR_F, 0.3)) break;
		    //printf(current y distance);
		    if(!task(T_TURN, speed, ODOMETRY, M_PI)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed, LINE, LINE_RIGHT)) break;
		    if(!task(T_TURN, speed, ODOMETRY, M_PI)) break;
		    state = M_MOVE_OBSTACLE;
			finished = 0;
		    break;
		case M_MOVE_OBSTACLE:
		    if(!task(T_FOLLOW_STRAIGHT, speed, ODOMETRY, 2.5)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed, IR_F, 0.3)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed / 2, IR_F, 0.15)) break;
		    if(!task(T_FOLLOW_STRAIGHT, speed / 2, ODOMETRY, 0.55)) break;
		    if(!task(T_REVERSE, speed, ODOMETRY, 0.95)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI / 2)) break;
		    if(!task(T_FORWARD, speed / 2, LINE, LINE_CROSS)) break;
		    if(!task(T_OCTURN, speed / 2, ODOMETRY, -M_PI / 2)) break;
			if(!task(T_FOLLOW_STRAIGHT, speed, LINE, LINE_CROSS)) break;
		    state = M_FIND_GHOST_GATE;
			finished = 0;
		    break;
		case M_FIND_GHOST_GATE:
		    if(!task(T_FOLLOW_RIGHT, speed, LINE, LINE_CROSS)) break;
			int temp = task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY | IR_L, 1.5, 0.4);
			if(!temp) break;
			if(temp == IR_L) { //First rod found, looking for second
				if(!task(T_FORWARD, speed / 4, ODOMETRY, 0.05));
				temp = task(T_FORWARD, speed/4, ODOMETRY | IR_L, 0.5, 0.4);
				if(!temp) break;
				if(temp == IR_L) { //Second rod found, success!
					finished = 0;
					state = M_GO_THROUGH_GHOST_GATE;
					break;
				} else { //Second rod not found
					if(!task(T_REVERSE, speed/4, ODOMETRY, 0.6)) break; //Reverse to before first rod
					temp = task(T_REVERSE, speed/4, ODOMETRY | IR_L, 0.5, 0.4); //look for second rod before first
					if(!temp) break;
					if(temp == IR_L) {//Second rod found behind first
						if(!task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY, 0.45)) break;
						finished = 0;
						state = M_GO_THROUGH_GHOST_GATE;
						break;
					}
				}
			} else if (temp == ODOMETRY){ //No rods found
				temp = task(T_REVERSE, speed/4, ODOMETRY | IR_L, 1.5, 0.4);
				if(!temp) break;
				if(temp == IR_L) { // First rod found while backing up
					if(!task(T_REVERSE, speed / 4, ODOMETRY, 0.05)) break;
					temp = task(T_REVERSE, speed/4, ODOMETRY | IR_L, 0.5, 0.4);
					if(!temp) break;
					if(temp == IR_L) { //Second rod found while backing up
						if(!task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY, 0.45)) break;
						finished = 0;
						state = M_GO_THROUGH_GHOST_GATE;
						break;
					} else { //Second rod not found while backing up
						if(!task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY, 0.6)) break;
						temp = task(T_FOLLOW_STRAIGHT, speed/4, ODOMETRY | IR_L, 0.5, 0.4);
						if(!temp) break;
						if(temp == IR_L) {//Second rod found ahead of first
							finished = 0;
							state = M_GO_THROUGH_GHOST_GATE;
							break;
						}
					}
				}
			}
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
			int temp;
			temp = task(T_FOLLOW_STRAIGHT, speed, LINE | IR_L, LINE_CROSS, 0.15);
			if(!temp) break;
			if(temp == LINE) {
				temp = task(T_REVERSE, speed/4, ODOMETRY | IR_L, 0.50, 0.15);
				if(!temp) break;
				if(temp == ODOMETRY) {
					temp = task(T_FOLLOW_STRAIGHT, speed, LINE | IR_L, LINE_CROSS, 0.15);
					if(!temp) break;
				}
			} 
			if(temp == IR_L) {
				if(!task(T_FORWARD, speed, ODOMETRY, 0.40)) break;
				if(!task(T_TURN, speed, ODOMETRY, -M_PI/2)) break;
				//TODO: Consider the task_status!
				int task_status;
				do {
					if((task_status = !task(T_FOLLOW_WALL, speed/2, IR_L, 0.30))) break;
				} while(task_status == IR_L);
				if(!task(T_FORWARD, speed/2, ODOMETRY, 0.225)) break;
				if(!task(T_TURN, speed/2, ODOMETRY, -M_PI/2)) break;
				if(!task(T_FORWARD, speed/2, ODOMETRY, 0.3)) break;
				if(!task(T_TURN, speed/2, ODOMETRY, -M_PI/2)) break;
				do{
					if((task_status = !task(T_FOLLOW_WALL, speed/2, IR_L))) break;
				} while(task_status==IR_L);
				if(!task(T_FORWARD, speed/2, ODOMETRY, 0.225)) break;
				if(!task(T_TURN, speed/2, ODOMETRY, -M_PI/2)) break;
				if(!task(T_FORWARD, speed, LINE, LINE_CROSS)) break;
				
				break;
			}
			finished = 0;
			state = M_WHITE_IS_THE_NEW_BLACK;
		    break;
		}
		case M_WHITE_IS_THE_NEW_BLACK:
			if(!task(T_FOLLOW_STRAIGHT, speed, ODOMETRY, 0.2)) break;
			if(!task(T_TURN, speed, ODOMETRY, -M_PI/2)) break;
			if(!task(T_FORWARD, speed, LINE, WHITE_LINE)) break;
			if(!task(T_FOLLOW_WHITE_STRAIGHT, speed, LINE, LINE_CROSS)) break;
			if(!task(T_FORWARD, speed, ODOMETRY, 0.3)) break;
			if(!task(T_TURN, speed, ODOMETRY, M_PI/2)) break;
			finished = 0;
			state = M_HARDCORE_PARKING_ACTION;
		    break;
		case M_HARDCORE_PARKING_ACTION: {
			if(!task(T_FOLLOW_STRAIGHT, speed/2, IR_F, 0.15)) break;
			if(!task(T_OCTURN, speed/2, ODOMETRY, -M_PI/2)) break;
			if(!task(T_FORWARD, speed/2, ODOMETRY, 0.4)) break;
			if(!task(T_OCTURN, speed/2, ODOMETRY, M_PI/2)) break;
			if(!task(T_FORWARD, speed/2, ODOMETRY, 0.55)) break;
			if(!task(T_ROCTURN, speed/2, ODOMETRY, -M_PI/2)) break;
			if(!task(T_ROCTURN, speed/2, ODOMETRY, M_PI/2)) break;
			if(!task(T_REVERSE, speed/2, ODOMETRY, 0.60)) break;
			if(!task(T_ROCTURN, speed/2, ODOMETRY, M_PI/2)) break;
			if(!task(T_FORWARD, speed/2, LINE, LINE_CROSS)) break;
			if(!task(T_FORWARD, speed/2, ODOMETRY, 0.15)) break;
			if(!task(T_OCTURN, speed/2, ODOMETRY, -M_PI/2)) break;
			if(!task(T_FORWARD, speed/2, IR_F, 0.15)) break;
		    break;
		}
		}//End of switch.
	}
    while(!finished);

    printf("Ending mission.\n");
    task(T_STOP, 0, 0);
}
