#include <time.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include "../motion/motion.h"
#include "../odometry/odometry.h"
#include "task.h"

int
task(int task_id, int speed, int triggers, ...)
{
    va_list arguments;
	int terminator = 0;

    long task_start = time(NULL);
    double current_distance = 0.0;

    double time;		//in seconds
    double distance;		//in centimeters
    int line;			//special case for the line sensor*/
    double ir_distance[5];	//in centimeters

    if(triggers & TIME)
	{
	    time = va_arg(arguments, double);
	}

    if(triggers & ODOMETRY)
	{
	    distance = va_arg(arguments, double);
	}

    if(triggers & LINE)
	{
	    line = va_arg(arguments, int);
	}

    if(triggers & IR_F_AVG)
	{
	    ir_distance = va_arg(arguments, double);
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

    while(task_id != T_FINISHED)
	{
	    //Synchronize and update odometry.
	    rhdSync();
	    update_odometry(get_general_odometry());
	    current_odometry.left_encoder = out.encoder_left->data[0];
	    current_odometry.right_encoder = out.encoder_right->data[0];
	    update_odometry(&current_odometry);

	    current_distance = current_odometry.x;
	    printf("r, l is:  %d,  %d\n", out.encoder_right->data[0],
		   out.encoder_left->data[0]);
	    printf("x, y is:  %f,  %f\n", current_distance,
		   current_odometry.y);

	    //Sensor Checking, and reactions
	    if(triggers & TIME)
		{
		    if(time(NULL) >= task_start + time)
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
				   current_distance - distance);
			    if(current_distance - distance >= 0)
				{
					terminator = TIME;
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
			forward(speed, current_distance, distance);
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
			terminator = 0;
		}
	}

    //Remember to inform the hardware about the recent changes.


    return terminator;
}
