
#ifndef _task_h
#define _task_h

#include <stdlib.h>
#include "../main.h"
#include "../odometry/odometry.h"

enum task_states
{ T_FORWARD, T_TURN, T_OCTURN, T_REVERSE, T_WAIT, T_FOLLOW, T_FOLLOW_RIGHT,
    T_FOLLOW_STRAIGHT, T_FOLLOW_LEFT, T_STOP, T_FINISHED
};

enum
{ TIME = 1, ODOMETRY = 1 << 1, LINE = 1 << 2, IR_L = 1 << 3, IR_FL =
	1 << 4, IR_FC = 1 << 5, IR_FR = 1 << 6, IR_R = 1 << 7, IR_F =
	1 << 8, IR_F_AVG, 1 << 9
};

typedef struct
{
    int speed;			//set the maxspeed
    int triggers;		//Which sensors should trigger state change
    double distance;		//in centimeters
    double ir_distance[5];	//in centimeters
    double time;		//in seconds
    int line;			//special case for the line sensor
} task_parameters;

/* Call with, Task ID, Speed, Triggers, Odometry Trigger, Time Trigger,
 * Line Sensor Trigger, IR Triggers (sensors are ordered from left to right)
 */
int task(int task_id, int speed, int triggers, ...);

#endif
