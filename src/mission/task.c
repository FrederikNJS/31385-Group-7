#include <time.h>
#include <stdio.h>
#include <math.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include "../motion/motion.h"
#include "../odometry/odometry.h"
#include "../linesensor/linesensor.h"
#include "../infrared/infrared.h"
#include "task.h"

typedef double *double_pointer_yeah;
int
task(int task_id, int speed, int triggers, ...)
{
    task_data_t task_data;

    //Initialize the data for the current task,
    //updating goal x, y and angle in parameters.
    //init_task_data(task_id, parameters, &task_data);
    init_task_data(task_id, NULL, &task_data);

    double time = 0;		//in seconds
    int line;			//special case for the line sensor*/
    double ir_distance[5];	//in meters

    int ir_updated = 0;

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

    else if(triggers & IR_F)
	{
	    ir_distance[1] = ir_distance[2] = ir_distance[3] =
		va_arg(arguments, double);
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

	    printf("Current task id: %d\n", task_id);

	    //Synchronize and update odometry.
	    rhdSync();
	    update_odometry(&current_odometry);
	    update_task_data(&task_data);

	    //Check for ir-sensor measurement.
	    if((triggers &
		(IR_L || IR_FL || IR_FC || IR_FR || IR_R || IR_F || IR_F_AVG)
		|| task_id == T_FOLLOW_WALL)
	       && task_data.current_time -
	       task_data.last_ir_sensor_measure_time > 0.200)
		{
		    ir_updated = 1;
		}

	    printf("r, l is:  %d,  %d\n", out.encoder_right->data[0],
		   out.encoder_left->data[0]);
	    printf("x, y is:  %f,  %f\n", current_odometry.x,
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
		    if(task_id & T_TURN || task_id & T_OCTURN)
			{
			    if(absanglediff
			       (current_odometry.angle,
				task_data.start_angle) >
			       absd(task_data.goal_distance))
				{
				    terminator = ODOMETRY;
				    task_id = T_FINISHED;
				}
			}
		    else
			{
			    printf("In check for length: %f \n",
				   task_data.current_distance -
				   task_data.goal_distance);
			    if(task_data.current_distance -
			       task_data.goal_distance >= 0)
				{
				    terminator = ODOMETRY;
				    task_id = T_FINISHED;
				}
			}

		}
	    if(triggers & LINE)
		{
		    double lines[2];
		    int line_case = find_line_position(BLACK_LINE, lines);
		    if(line_case == line || ((line == -4) && line_case))
			{
			    terminator = ODOMETRY;
			    task_id = T_FINISHED;
			}
		}

	    if(ir_updated)
		{
		    if(triggers & IR_L)
			{
			    if(ir_closer_than(0, ir_distance[0]))
				{
				    terminator = IR_L;
				    task_id = T_FINISHED;
				}
			}
		    if(triggers & IR_FL || triggers & IR_F)
			{
			    if(ir_closer_than(0, ir_distance[0]))
				{
				    terminator = IR_FL;
				    task_id = T_FINISHED;
				}
			}
		    if(triggers & IR_FC || triggers & IR_F)
			{
			    if(ir_closer_than(0, ir_distance[0]))
				{
				    terminator = IR_FC;
				    task_id = T_FINISHED;
				}
			}
		    if(triggers & IR_FR || triggers & IR_F)
			{
			    if(ir_closer_than(0, ir_distance[0]))
				{
				    terminator = IR_FR;
				    task_id = T_FINISHED;
				}
			}
		    if(triggers & IR_R)
			{
			    if(ir_closer_than(0, ir_distance[0]))
				{
				    terminator = IR_R;
				    task_id = T_FINISHED;
				}
			}

		    if(task_id != T_FOLLOW_WALL)
			{
			    ir_updated = 0;
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
		    turn(speed, current_odometry.angle,
			 task_data.goal_distance, task_data.start_angle);
		    break;
		case T_OCTURN:
		    octurn(speed, current_odometry.angle,
			   task_data.goal_distance);
		    break;
		case T_REVERSE:
		    reverse(speed, &task_data);
		    break;
		case T_WAIT:
		    in.speed_left->data[0] = 0;
		    in.speed_left->updated = 1;
		    in.speed_right->data[0] = 0;
		    in.speed_right->updated = 1;
		    break;
		case T_FOLLOW_RIGHT:
		    follow_line(GO_RIGHT, speed, BLACK_LINE);
		    break;
		case T_FOLLOW_STRAIGHT:
		    follow_line(GO_STRAIGHT, speed, BLACK_LINE);
		    break;
		case T_FOLLOW_LEFT:
		    follow_line(GO_LEFT, speed, BLACK_LINE);
		    break;
		case T_FOLLOW_WHITE_RIGHT:
		    follow_line(GO_RIGHT, speed, WHITE_LINE);
		    break;
		case T_FOLLOW_WHITE_STRAIGHT:
		    follow_line(GO_STRAIGHT, speed, WHITE_LINE);
		    break;
		case T_FOLLOW_WHITE_LEFT:
		    follow_line(GO_LEFT, speed, WHITE_LINE);
		    break;
		case T_FOLLOW_WALL:

		    if(ir_updated == 1)
			{
			    ir_updated = 0;
			}
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
	    //printf("Before keyboard.\n");
	    ioctl(0, FIONREAD, &arg);
	    if(arg != 0 && task_id != T_STOP)
		{
		    task_id = T_FINISHED;
		    terminator = 0;
		    //printf("Stopping.\n");
		}
	    //printf("After keyboard.\n");
	}

    //If the task didn't use the goal data, they need to be updated.
    if(!task_data.uses_goal)
	{
	    /*parameters->goal_x = current_odometry.x;
	       parameters->goal_y = current_odometry.y;
	       parameters->goal_angle = current_odometry.angle; */
	}

    //Remember to inform the hardware about the recent changes.

    //Cleanup!
    va_end(arguments);

    return terminator;
}

void
init_task_data(int task_id, task_parameters * parameters,
	       task_data_t * task_data)
{

    task_data->current_distance = 0.;
    task_data->current_tick = 0;
    task_data->start_time = task_data->last_ir_sensor_measure_time =
	task_data->current_time = time(NULL);
    task_data->last_ir_sensor_measure_time += 0.2;
    task_data->goal_distance = 1000000;
    int uses_goal = 0;

    //If goal is used, enable uses_goal and set the goal pos/angle.
    //TODO: Implement.
    switch (task_id)
	{
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

void
update_task_data(task_data_t * task_data)
{

    task_data->current_tick++;
    task_data->current_time = time(NULL);
    task_data->current_distance += current_odometry.dU;
}
