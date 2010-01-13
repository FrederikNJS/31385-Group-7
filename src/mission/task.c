#include "task.h"
#include "time.h"

int
task(int task_id, task_parameters parameters)
{
    int task_finished = 0;
    long task_start = time(NULL);
    while(!task_finished)
	{
	    rhdSync();
	    //Sensor Checking, and reactions
	    if(sensor_trigger & TIME)
		{
		    if(time(NULL) >= task_start + time_trigger)
			{
			    task_id = FINISHED;
			}
		}
	    if(sensor_trigger & ODOMETRY)
		{
		    if(odometry() >= 0 /*odometry distance*/)
			{
				task_id = FINISHED;
			}
		}

	    //Task State Machine
	    switch (task_id)
		{
		case FORWARD:
		    forward();
		    break;
		case TURN:
		    break;
		case REVERSE:
		    break;
		case WAIT:
		    break;
		case FOLLOW:
		    break;
		case FOLLOW_RIGHT:
		    break;
		case FOLLOW_STRAIGHT:
		    break;
		case FOLLOW_LEFT:
		    break;
		case STOP:
		    break;
		case FINISHED:
		    break;
		}
	}
}
