#include <time.h>
#include "task.h"
#include "../odometry/odometry.h"
#include "../motion/motion.h"

int task(int task_id, task_parameters * parameters, input * in, output * out)
{
    int task_finished = 0;
    long task_start = time(NULL);
    while(!task_finished)
	{
	    rhdSync();
	    //Sensor Checking, and reactions
	    if(parameters->triggers & TIME)
		{
		    if(time(NULL) >= task_start + parameters->time)
			{
			    task_id = T_FINISHED;
			}
		}
	    if(parameters->triggers & ODOMETRY)
		{
		    if(parameters->distance >= 0 /*odometry distance*/)
			{
				task_id = T_FINISHED;
			}
		}

	    //Task State Machine
	    switch (task_id)
		{
		case T_FORWARD:
		    forward(parameters->speed);
		    break;
		case T_TURN:
		    break;
		case T_REVERSE:
		    break;
		case T_WAIT:
		    break;
		case T_FOLLOW:
		    break;
		case T_FOLLOW_RIGHT:
		    break;
		case T_FOLLOW_STRAIGHT:
		    break;
		case T_FOLLOW_LEFT:
		    break;
		case T_STOP:
		    break;
		case T_FINISHED:
		    break;
		}
	}
	return 0;
}
