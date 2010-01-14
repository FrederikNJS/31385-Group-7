#include <time.h>
#include <stdio.h>

#include "../motion/motion.h"
#include "../odometry/odometry.h"

#include "task.h"

int
task(int task_id, task_parameters * parameters)
{
    long task_start = time(NULL);
	double current_distance = 0.0;
    while(task_id != T_FINISHED)
	{
		//Synchronize and update odometry.
	    rhdSync();
	    current_odometry.left_encoder = out.encoder_left->data[0];
    	current_odometry.right_encoder = out.encoder_right->data[0];
		update_odometry(&current_odometry);

		current_distance = current_odometry.x;
		printf("r, l is:  %d,  %d\n", out.encoder_right->data[0], out.encoder_left->data[0]);
		printf("x, y is:  %f,  %f\n", current_distance, current_odometry.y);

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
			printf("In check for length: %f \n", current_distance - parameters->distance);
		    if(current_distance - parameters->distance >= 0 /*odometry distance */ )
			{
			    task_id = T_FINISHED;
			}
			
		}
		if(parameters->triggers & LINE) {
			
		}
		if(parameters->triggers & INFRARED) {
			
		}

	    //Task State Machine
	    switch (task_id)
		{
		case T_FORWARD:
			printf("Forward.\n");
		    forward(parameters->speed, current_distance, parameters->distance);
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

	//Remember to inform the hardware about the recent changes.


    return 0;
}
