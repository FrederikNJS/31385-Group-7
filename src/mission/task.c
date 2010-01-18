#include <time.h>
#include <stdio.h>
#include <math.h>
#include <sys/ioctl.h>

#include "../motion/motion.h"
#include "../odometry/odometry.h"

#include "task.h"

int
task(int task_id, task_parameters * parameters)
{
	task_data_t task_data;
	
	//Initialize the data for the current task,
	//updating goal x, y and angle in parameters.
	init_task_data(task_id, parameters, &task_data);

    while(task_id != T_FINISHED)
	{
	    //Synchronize and update odometry.
	    rhdSync();
	    current_odometry.left_encoder = out.encoder_left->data[0];
	    current_odometry.right_encoder = out.encoder_right->data[0];
	    update_odometry(&current_odometry);
		update_task_data(&task_data);

	    printf("r, l is:  %d,  %d\n", out.encoder_right->data[0],
		   out.encoder_left->data[0]);
	    printf("x, y is:  %f,  %f\n", task_data.current_distance,
		   current_odometry.y);

	    //Sensor Checking, and reactions
	    if(parameters->triggers & TIME)
		{
		    if(task_data.current_time >= task_data.start_time + parameters->time)
			{
			    task_id = T_FINISHED;
			}
		}
	    if(parameters->triggers & ODOMETRY)
		{
		    if(task_id & T_TURN) {}


			else
		    if(task_id & T_OCTURN)
			{

			}
		    else
			{
			    printf("In check for length: %f \n",
				   task_data.current_distance - parameters->distance);
			    if(task_data.current_distance - parameters->distance >= 0)
				{

				}
			}

		}
	    if(parameters->triggers & LINE)
		{

		}
	    if(parameters->triggers & INFRARED)
		{

		}

	    //Task State Machine
	    switch (task_id)
		{
		case T_FORWARD:
		    printf("Forward.\n");
		    forward(parameters->speed, parameters->distance, &task_data);
		    break;
		case T_TURN:
		    break;
		case T_OCTURN:
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

	    //Stop if keyboard is activated
	    void *arg;
	    ioctl(0, FIONREAD, &arg);
	    if(arg != 0)
		{
		    task_id = T_FINISHED;
		}
	}

	//If the task didn't use the goal data, they need to be updated.
	if (!task_data.uses_goal) {
		parameters->goal_x = current_odometry.x;
		parameters->goal_y = current_odometry.y;
		parameters->goal_angle = current_odometry.angle;
	}

    //Remember to inform the hardware about the recent changes.

    return 0;
}

void init_task_data(int task_id, task_parameters * parameters, task_data_t * task_data) {
	
	task_data->current_distance = 0.;
	task_data->current_tick = 0;
	task_data->start_time = task_data->current_time = time(NULL);
	
	int uses_goal = 0;

	//TODO: Implement.
	//If goal is used, enable uses_goal and set the goal pos/angle.	
	switch (task_id) {
		case T_FORWARD:
			task_data->goal_distance = parameters->distance;
			uses_goal = 1;
			parameters->goal_angle = parameters->goal_angle;
			parameters->goal_x = parameters->goal_x + task_data->goal_distance * cos(parameters->goal_angle);
			parameters->goal_y = parameters->goal_y + task_data->goal_distance * sin(parameters->goal_angle);
		    break;
		case T_TURN:
		    break;
		case T_OCTURN:
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

	task_data->uses_goal = uses_goal;
	task_data->start_x = current_odometry.x;
	task_data->start_y = current_odometry.y;
	task_data->start_angle = current_odometry.angle;
}

void update_task_data(task_data_t * task_data) {

	task_data->current_tick++;
	task_data->current_time = time(NULL);
	task_data->current_distance += current_odometry.dU;

}
