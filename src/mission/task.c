#include <time.h>
#include <stdio.h>
#include <math.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include "../motion/motion.h"
#include "../odometry/odometry.h"
#include "task.h"

typedef double* double_pointer_yeah;
int
task(int task_id, int speed, int triggers, ...)
{
	task_data_t task_data;
	
	//Initialize the data for the current task,
	//updating goal x, y and angle in parameters.
	//init_task_data(task_id, parameters, &task_data);
	init_task_data(task_id, NULL, &task_data);

    long task_start = time(NULL);
    double current_distance = 0.0;

    double time = 0;		//in seconds
    int line;			//special case for the line sensor*/
    double ir_distance[5];	//in meters
	double * ir_distance_p = ir_distance;

	int terminator = -1;

	va_list arguments;
	va_start(arguments, triggers);

    if(triggers & TIME)
	{
	    time = va_arg(arguments, double);
	}

    if(triggers & ODOMETRY)
	{
	    task_data.goal_distance = va_arg(arguments, double);
	}

    if(triggers & LINE)
	{
	    line = va_arg(arguments, int);
	}

    if(triggers & IR_F_AVG)
	{
	    ir_distance_p = va_arg(arguments, double_pointer_yeah);
	}
    else if(triggers & IR_F)
	{
	    ir_distance[1] = ir_distance[2] = ir_distance[3] =
		va_arg(arguments, double);
	}
    else if(triggers & IR_F_AVG)
	{
	    ir_distance[0] = va_arg(arguments, double);
	}
    else
	{
	    if(triggers & IR_L)
		{
		    ir_distance[0] = va_arg(arguments, double);
		}
	    else if(triggers & IR_FL)
		{
		    ir_distance[1] = va_arg(arguments, double);
		}
	    else if(triggers & IR_FC)
		{
		    ir_distance[2] = va_arg(arguments, double);
		}
	    else if(triggers & IR_FR)
		{
		    ir_distance[3] = va_arg(arguments, double);
		}
	    else if(triggers & IR_R)
		{
		    ir_distance[4] = va_arg(arguments, double);
		}
	}

	while(task_id != T_STOP)
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

		//printf("Value of random variables. Shouldn't be null...%p\n", parameters);

	    //Sensor Checking, and reactions
	    if(triggers & TIME)
		{
			if(task_data.current_time >= task_data.start_time + time)
			{
			    task_id = T_FINISHED;
				terminator = TIME;
			}
		}
	    if(triggers & ODOMETRY)
		{
		    if(task_id & T_TURN)
			{
				
			}


		    else if(task_id & T_OCTURN)
			{

			}
		    else
			{
			    printf("In check for length: %f \n",
				   current_distance - task_data.goal_distance);
			    if(current_distance - task_data.goal_distance >= 0)
				{
					terminator = ODOMETRY;
					task_id= T_FINISHED;
				}
			}

		}
	    if(triggers & LINE)
		{
		    
		}
	    if(triggers & IR_F_AVG)
		{
		   
		}
	    else if(triggers & IR_F)
		{
		    
		}
	    else if(triggers & IR_F_AVG)
		{
		    
		}
	    else
		{
		    if(triggers & IR_L)
			{
			   
			}
		    else if(triggers & IR_FL)
			{
			    
			}
		    else if(triggers & IR_FC)
			{
			    
			}
		    else if(triggers & IR_FR)
			{
			    
			}
		    else if(triggers & IR_R)
			{
			    
			}
		}

	    //Task State Machine
	    switch (task_id)
		{
		case T_FORWARD:
		    printf("Forward.\n");
		    forward(speed, &task_data);
		    break;
		case T_TURN:
			turn(speed, current_odometry.angle, task_data.goal_distance);
		    break;
		case T_OCTURN:
		    break;
		case T_REVERSE:
		    break;
		case T_WAIT:
		    break;
		case T_FOLLOW_RIGHT:
			/*follow_line(GO_RIGHT, speed, LINE_BLACK);*/
		    break;
		case T_FOLLOW_STRAIGHT:
			/*follow_line(GO_STRAIGHT, speed, LINE_BLACK);*/
		    break;
		case T_FOLLOW_LEFT:
			/*follow_line(GO_LEFT, speed, LINE_BLACK);*/
		    break;
		case T_FOLLOW_WHITE_RIGHT:
			/*follow_line(GO_RIGHT, speed, LINE_WHITE);*/
		    break;
		case T_FOLLOW_WHITE_STRAIGHT:
			/*follow_line(GO_STRAIGHT, speed, LINE_WHITE);*/
		    break;
		case T_FOLLOW_WHITE_LEFT:
			/*follow_line(GO_LEFT, speed, LINE_WHITE);*/
		    break;
		case T_STOP:
			printf("STOP\n");
		    break;
		case T_FINISHED:
		    in.speed_left->data[0] = 0;
		    in.speed_left->updated = 1;
		    in.speed_right->data[0] = 0;
		    in.speed_right->updated = 1;
			task_id = T_STOP;
		    break;
		}

	    //Stop if keyboard is activated
	    void *arg;
		printf("Before keyboard.\n");
	    ioctl(0, FIONREAD, &arg);
	    if(arg != 0 && task_id != T_STOP)
		{
		    task_id = T_FINISHED;
			terminator = 0;
			printf("Stopping.\n");
		}
		printf("After keyboard.\n");
	}

	//If the task didn't use the goal data, they need to be updated.
	if (!task_data.uses_goal) {
		/*parameters->goal_x = current_odometry.x;
		parameters->goal_y = current_odometry.y;
		parameters->goal_angle = current_odometry.angle;*/
	}

    //Remember to inform the hardware about the recent changes.

	//Cleanup!
	va_end(arguments);

    return terminator;
}

void init_task_data(int task_id, task_parameters * parameters, task_data_t * task_data) {
	
	task_data->current_distance = 0.;
	task_data->current_tick = 0;
	task_data->start_time = task_data->current_time = time(NULL);
	task_data->goal_distance = 1000000;
	int uses_goal = 0;

	//If goal is used, enable uses_goal and set the goal pos/angle.
	//TODO: Implement.
	switch (task_id) {
		case T_FORWARD:
			//task_data->goal_distance = parameters->distance;
			uses_goal = 1;
			//parameters->goal_angle = parameters->goal_angle;
			//parameters->goal_x = parameters->goal_x + task_data->goal_distance * cos(parameters->goal_angle);
			//parameters->goal_y = parameters->goal_y + task_data->goal_distance * sin(parameters->goal_angle);
		    break;
		case T_TURN:
		    break;
		case T_OCTURN:
		    break;
		case T_REVERSE:
		    break;
		case T_WAIT:
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
