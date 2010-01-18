
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
{ TIME = 1, ODOMETRY = 1 << 1, LINE = 1 << 2, INFRARED = 1 << 3 };

//Guide to parameters and task_data
//
//The task parameters holds the parameters for the
//current task. In general, the tasks do not edit
//this file; however, the goal_* variables
//are edited by the task correspondingly to how the
//task should edit them.

typedef struct
{
    int speed;			//set the maxspeed
    int triggers;		//Which sensors should trigger state change
    double distance;		//in meters
    double ir_distance[5];	//in meters
    double time;		//in seconds
    int line;			//special case for the line sensor
	/**The goal variables are used to keep track
	 * of the ideal end state after a task.
	 * They are updated by the task.
	 */
	double goal_x, goal_y, goal_angle;
} task_parameters;

typedef struct {
	double current_distance, goal_distance;
	//Tick is incremented once each rhdSync,
	//and is set to zero at the start of a task.
	int current_tick;
	//The start time as given by "time" from <time.h>
	double start_time;
	//The current time as given by "time" from <time.h>
	double current_time;
	//The position/angle at the start.
	double start_x, start_y, start_angle;
	//Whether or not the goal variables are used in the task.
	int uses_goal;
} task_data_t;

int task(int task_id, task_parameters * parameters);
void init_task_data(int task_id, task_parameters * parameters, task_data_t * task_data);
void update_task_data(task_data_t * task_data);

#endif
