#include "motion.h"
#include "../main.h"

void
forward(int speed, double current_distance, double expected_distance)
{
    extern input in;

    mod_speed = speed_calc(speed, current_distance, expected_distance);

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

double
speed_calc(double max_speed, double current_distance, double expected_distance)
{
    double speed = 200*current_distance;
    double speed2 = 200*(expected_distance-current_distance);

    if(speed <= 0 || speed2 <= 0 || speed2 != speed2)
	{
	    //printf("speed0:     0\n");
	    return 0;
	}
    if(speed <= max_speed && speed <= speed2)
	{
	    //printf("speed:     %f\n", speed);
	    return (double) speed;
	}
    if(max_speed <= speed && max_speed <= speed2)
	{
	    //printf("max_speed: %f\n", (double) max_speed);
	    return (double) max_speed;
	}
    //printf("speed2:    %f\n", speed2);
    return (double) speed2;
}
