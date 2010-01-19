#include <stdio.h>
#include <math.h>
#include "motion.h"
#include "../main.h"
#include "../linesensor/linesensor.h"
#include "../calibration/calibration.h"

void
forward(int speed, double expected_distance, task_data_t * td)
{
    extern input in;
    int mod_speed = speed_calc(speed, td->current_distance, expected_distance);

    printf
	("In forward, mod_speed is: %d,  speed is:  %d,  curr. dist is: %f,  exp. dist: %f\n",
	 mod_speed, speed, td->current_distance, expected_distance);

    double speed_diff = 100.0 * (td->start_angle - current_odometry.angle);

    if(in.speed_left->data[0] != mod_speed - speed_diff)
	{
	    in.speed_left->data[0] = mod_speed - speed_diff;
	    in.speed_left->updated = 1;
	}
    if(in.speed_right->data[0] != mod_speed + speed_diff)
	{
	    in.speed_right->data[0] = mod_speed + speed_diff;
	    in.speed_right->updated = 1;
	}
}

void
turn(int speed, double current_angle, double expected_angle)
{
    extern input in;

    if(expected_angle < 0)
	{
	    int mod_speed =
		speed_calc(speed, -0.00232555523517358448 * current_angle,
			   -0.00232555523517358448 * expected_angle);

	    if(in.speed_left->data[0] != mod_speed)
		{
		    in.speed_left->data[0] = -mod_speed;
		    in.speed_left->updated = 1;
		}
	    if(in.speed_right->data[0] != mod_speed)
		{
		    in.speed_right->data[0] = mod_speed;
		    in.speed_right->updated = 1;
		}
	}
    else
	{
	    int mod_speed =
		speed_calc(speed, 0.00232555523517358448 * current_angle,
			   0.00232555523517358448 * expected_angle);

	    if(in.speed_left->data[0] != mod_speed)
		{
		    in.speed_left->data[0] = mod_speed;
		    in.speed_left->updated = 1;
		}
	    if(in.speed_right->data[0] != mod_speed)
		{
		    in.speed_right->data[0] = -mod_speed;
		    in.speed_right->updated = 1;
		}
	}
}

void
off_center_turn(int speed, double current_angle, double expected_angle)
{
    extern input in;

    if(expected_angle < 0)
	{
	    int mod_speed =
		speed_calc(speed, -0.00465111047034716895 * current_angle,
			   -0.00465111047034716895 * expected_angle);

	    if(in.speed_left->data[0] != 0)
		{
		    in.speed_left->data[0] = 0;
		    in.speed_left->updated = 1;
		}
	    if(in.speed_right->data[0] != mod_speed)
		{
		    in.speed_right->data[0] = mod_speed;
		    in.speed_right->updated = 1;
		}
	}
    else
	{
	    int mod_speed =
		speed_calc(speed, 0.00465111047034716895 * current_angle,
			   0.00465111047034716895 * expected_angle);

	    if(in.speed_left->data[0] != mod_speed)
		{
		    in.speed_left->data[0] = mod_speed;
		    in.speed_left->updated = 1;
		}
	    if(in.speed_right->data[0] != 0)
		{
		    in.speed_right->data[0] = 0;
		    in.speed_right->updated = 1;
		}
	}
}

double
speed_calc(double max_speed, double current_distance,
	   double expected_distance)
{
    double speed = 80*sqrt(current_distance) + 1;
    double speed2 = 80*sqrt((expected_distance - current_distance));

    if(speed <= 0 || speed2 <= 0 || speed2 != speed2)
	{
	    printf("speed0:     0\n");
	    return 0;
	}
    if(speed <= max_speed && speed <= speed2)
	{
	    printf("speed:     %f\n", speed);
	    return (double) speed;
	}
    if(max_speed <= speed && max_speed <= speed2)
	{
	    printf("max_speed: %f\n", (double) max_speed);
	    return (double) max_speed;
	}
    printf("speed2:    %f\n", speed2);
    return (double) speed2;
}

void follow_line(int direction, double max_speed, int line_color) {
	double line[2];
	extern input in;
	int retval = find_line_position(line_color, line);
	in.speed_left->data[0] = max_speed;
	in.speed_right->data[0] = max_speed;
	switch(retval) {
		case LINE_SINGLE:
			//Same action for all, just follow the line
			if(line[0] > 3.5) {
				in.speed_left->data[0] *= (2-line[0]/3.5);
			} else {
				in.speed_right->data[0] *= line[0]/3.5;
			}
			break;
		case LINE_DOUBLE:
			//Straight have nothing to do here
			if(direction == GO_STRAIGHT) {
				if(line[direction == 1 ? 0 : 1] > 3.5) {
					in.speed_left->data[0] *= (2-line[direction == 1 ? 0 : 1]/3.5);
				} else {
					in.speed_right->data[0] *= line[direction == 1 ? 0 : 1]/3.5;
				}
			}
			break;
		case LINE_LEFT:
			//Only left have something to do here
			if(direction == GO_LEFT) {
				in.speed_left->data[0] = 0;
			}
			break;
		case LINE_RIGHT:
			//Only right have something to do here
			if(direction == GO_RIGHT) {
				in.speed_right->data[0] = 0;
			}
			break;
		case LINE_CROSS:
			//Straight have nothing to do here
			if(direction == GO_RIGHT) {
				in.speed_right->data[0] = 0;
			} else if(direction == GO_LEFT) {
				in.speed_left->data[0] = 0;
			}
			break;
		case LINE_NONE:
			in.speed_right->data[0] = 0;
			in.speed_left->data[0] = 0;
			break;
	}
	in.speed_left->updated = 1;
	in.speed_right->updated = 1;
}
