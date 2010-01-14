
#ifndef _task_h
#define _task_h

#include "../main.h"
#include <stdlib.h>

enum task_states
{ T_FORWARD, T_TURN, T_REVERSE, T_WAIT, T_FOLLOW, T_FOLLOW_RIGHT,
	T_FOLLOW_STRAIGHT,
    T_FOLLOW_LEFT, T_STOP, T_FINISHED
};

enum
{ TIME = 1, ODOMETRY = 1 << 1, LINE = 1 << 2, INFRARED = 1 << 3 };

typedef struct
{
    int speed;			//set the maxspeed
    int triggers;		//Which sensors should trigger state change
    double distance;		//in centimeters
    double ir_distance[5];	//in centimeters
    double time;		//in seconds
    int line;			//special case for the line sensor
} task_parameters;

int task(int task_id, task_parameters * parameters);

#endif
