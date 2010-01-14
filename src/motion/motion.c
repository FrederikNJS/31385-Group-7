#include <stdio.h>
#include "motion.h"
#include "../main.h"

void
forward(int speed, double current_distance, double expected_distance)
{
    extern input in;
    int mod_speed = speed_calc(speed, current_distance, expected_distance);

    printf
	("In forward, mod_speed is: %d,  speed is:  %d,  curr. dist is: %f,  exp. dist: %f\n",
	 mod_speed, speed, current_distance, expected_distance);
    if(in.speed_left->data[0] != mod_speed)
	{
	    in.speed_left->data[0] = mod_speed;
	    in.speed_left->updated = 1;
	}
    if(in.speed_right->data[0] != mod_speed)
	{
	    in.speed_right->data[0] = mod_speed;
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
    double speed = 200 * current_distance + 1;
    double speed2 = 200 * (expected_distance - current_distance);

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
