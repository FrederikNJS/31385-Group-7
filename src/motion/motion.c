#include <stdio.h>
#include <math.h>
#include "motion.h"
#include "../main.h"
#include "../linesensor/linesensor.h"
#include "../calibration/calibration.h"

double absd(double a) { return a >= 0 ? a : -a;}

double absanglediff(double angle1, double angle2) {
 // Rotate angle1 with angle2 so that the sought after
 // angle is between the resulting angle and the x-axis
 angle1 -= angle2;

 
 // "Normalize" angle1 to range [-180,180)
 while(angle1 < -M_PI)
   angle1 += 2*M_PI;
 while(angle1 >= M_PI)
   angle1 -= 2*M_PI;

 
 // angle1 has the signed answer, just "unsign it"
 return absd(angle1);
}

double anglediff(double angle1, double angle2) {
 // Rotate angle1 with angle2 so that the sought after
 // angle is between the resulting angle and the x-axis
 angle1 -= angle2;

 
 // "Normalize" angle1 to range [-180,180)
 while(angle1 < -M_PI)
   angle1 += 2*M_PI;
 while(angle1 >= M_PI)
   angle1 -= 2*M_PI;

 
 // angle1 has the signed answer, just "unsign it"
 return angle1;
}

void
forward(int speed, task_data_t * td)
{
    extern input in;
    int mod_speed = speed_calc(speed, td->current_distance, td->goal_distance);

	double speed_diff = 10.0 * (td->start_angle - current_odometry.angle);
	
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
reverse(int speed, task_data_t * td)
{
    int mod_speed = speed_calc(speed, td->current_distance, td->goal_distance);

	double speed_diff = 10.0 * (current_odometry.angle - td->start_angle);
	
    if(in.speed_left->data[0] != (mod_speed - speed_diff)*-1)
	{
	    in.speed_left->data[0] = (mod_speed - speed_diff)*-1;
	    in.speed_left->updated = 1;
	}
    if(in.speed_right->data[0] != (mod_speed + speed_diff)*-1)
	{
	    in.speed_right->data[0] = (mod_speed + speed_diff)*-1;
	    in.speed_right->updated = 1;
	}
}

void
turn(int speed, double current_angle, double expected_angle, double start_angle)
{

	//Original value: 0.00232555523517358448

	//The current angle:
	//current_angle.
	//The destination angle:
	double destination_angle = start_angle - expected_angle;

	//The difference, signed.
	double diff = anglediff(current_angle, destination_angle);

	double direction_turn_around = diff >= 0. ? 1. : -1.;

	int mod_speed =
		speed_calc(speed, 0.132555523517358448 * absanglediff(current_angle, start_angle),
			   absd(0.132555523517358448 * expected_angle));

    if(in.speed_left->data[0] != direction_turn_around*mod_speed)
		{
		    in.speed_left->data[0] = direction_turn_around*mod_speed;
		    in.speed_left->updated = 1;
		}
    if(in.speed_right->data[0] != -direction_turn_around*mod_speed)
		{ 
		    in.speed_right->data[0] = -direction_turn_around*mod_speed;
		    in.speed_right->updated = 1;
		}
}

void
off_center_turn_reverse(int speed, double current_angle, double expected_angle, double start_angle)
{
	//Original value: 0.00232555523517358448

	//The current angle:
	//current_angle.
	//The destination angle:
	double destination_angle = start_angle - expected_angle;

	//The difference, signed.
	double diff = anglediff(current_angle, destination_angle);

	double direction_turn_around = diff >= 0. ? 0. : 1.;

	int mod_speed =
		speed_calc(speed, 0.132555523517358448 * absanglediff(current_angle, start_angle),
			   absd(0.132555523517358448 * expected_angle));

    if(in.speed_left->data[0] != -direction_turn_around*mod_speed)
		{
		    in.speed_left->data[0] = -direction_turn_around*mod_speed;
		    in.speed_left->updated = 1;
		}
    if(in.speed_right->data[0] != -(1-direction_turn_around)*mod_speed)
		{ 
		    in.speed_right->data[0] = -(1-direction_turn_around)*mod_speed;
		    in.speed_right->updated = 1;
		}
}

void
off_center_turn(int speed, double current_angle, double expected_angle, double start_angle)
{
	//Original value: 0.00232555523517358448

	//The current angle:
	//current_angle.
	//The destination angle:
	double destination_angle = start_angle - expected_angle;

	//The difference, signed.
	double diff = anglediff(current_angle, destination_angle);

	double direction_turn_around = diff >= 0. ? 1. : 0;

	int mod_speed =
		speed_calc(speed, 0.132555523517358448 * absanglediff(current_angle, start_angle),
			   absd(0.132555523517358448 * expected_angle));

    if(in.speed_left->data[0] != direction_turn_around*mod_speed)
		{
		    in.speed_left->data[0] = direction_turn_around*mod_speed;
		    in.speed_left->updated = 1;
		}
    if(in.speed_right->data[0] != (1-direction_turn_around)*mod_speed)
		{ 
		    in.speed_right->data[0] = (1-direction_turn_around)*mod_speed;
		    in.speed_right->updated = 1;
		}
}

double
speed_calc(double max_speed, double current_distance,
	   double expected_distance)
{
    double speed = 80*sqrt(current_distance) + 1;
    double speed2 = 80*sqrt((expected_distance - current_distance))+1;
    if(speed <= 0 || speed2 <= 0 || speed2 != speed2)
	{
	    return 0;
	}
    if(speed <= max_speed && speed <= speed2)
	{
	    return (double) speed;
	}
    if(max_speed <= speed && max_speed <= speed2)
	{
	    return (double) max_speed;
	}
    return (double) speed2;
}

void follow_line(int direction, double max_speed, int line_color, task_data_t * td) {
	double line[2];
	int retval = find_line_position(line_color, line);
	line_speed_calculation(direction, max_speed, retval, line, td);
}

void line_speed_calculation(int direction, double max_speed, int situation, double line[2], task_data_t * td) {
    int mod_speed = speed_calc(max_speed, td->current_distance, td->goal_distance);
	in.speed_left->data[0] = mod_speed;
	in.speed_right->data[0] = mod_speed;
	switch(situation) {
		case LINE_SINGLE:
			//Same action for all, just follow the line
			if(line[0] > 3.5) {
				in.speed_right->data[0] *= (2.0-line[0]/3.5);
			} else {
				in.speed_left->data[0] *= line[0]/3.5;
			}
			break;
		case LINE_DOUBLE:
			//Straight have nothing to do here
			if(direction != GO_STRAIGHT) {
				int tmp = direction == GO_RIGHT ? 1 : 0;
				if(line[tmp] > 3.5) {
					in.speed_right->data[0] *= (2.0-line[tmp]/3.5);
				} else {
					in.speed_left->data[0] *= line[tmp]/3.5;
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

