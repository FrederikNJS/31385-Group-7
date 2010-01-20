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

void
forward(int speed, task_data_t * td)
{
    extern input in;
    int mod_speed = speed_calc(speed, td->current_distance, td->goal_distance);

//    printf
//	("In forward, mod_speed is: %d,  speed is:  %d,  curr. dist is: %f,  exp. dist: %f\n",
//	 mod_speed, speed, td->current_distance, td->goal_distance);
	
	//printf("angle diff: %f\n", 10.0 * absanglediff(td->start_angle, current_odometry.angle));

    //double speed_diff = 10.0 * absanglediff(td->start_angle, current_odometry.angle);
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

//	printf("speed: %f\n", mod_speed + speed_diff);
//	printf("speed2: %f\n", mod_speed - speed_diff);

}

void
reverse(int speed, task_data_t * td)
{
    extern input in;
    int mod_speed = speed_calc(speed, td->current_distance, td->goal_distance);

//    printf
//	("In forward, mod_speed is: %d,  speed is:  %d,  curr. dist is: %f,  exp. dist: %f\n",
//	 mod_speed, speed, td->current_distance, td->goal_distance);

    double speed_diff = 100.0 * (td->start_angle - current_odometry.angle);

    if(in.speed_left->data[0] != mod_speed - speed_diff)
	{
	    in.speed_left->data[0] = (mod_speed - speed_diff)*-1;
	    in.speed_left->updated = 1;
	}
    if(in.speed_right->data[0] != mod_speed + speed_diff)
	{
	    in.speed_right->data[0] = (mod_speed + speed_diff)*-1;
	    in.speed_right->updated = 1;
	}
}

void
turn(int speed, double current_angle, double expected_angle, double start_angle)
{

	//Original value: 0.00232555523517358448

    if(expected_angle < 0)
	{
	    int mod_speed =
		speed_calc(speed, 0.132555523517358448 * absanglediff(current_angle, start_angle),
			   absd(0.132555523517358448 * expected_angle));
//		printf("Speed: %d\n", mod_speed);

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
		speed_calc(speed, 0.132555523517358448 * absanglediff(current_angle, start_angle),
			   absd(0.132555523517358448 * expected_angle));
//		printf("Speed: %d\n", mod_speed);

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
    double speed2 = 80*sqrt((expected_distance - current_distance))+1;
//	printf("speed: %f, speed2: %f, max_speed: %f, curr: %f exp: %f\n", speed, speed2, max_speed, current_distance, expected_distance);
    if(speed <= 0 || speed2 <= 0 || speed2 != speed2)
	{
//	    printf("speed0:     0\n");
	    return 0;
	}
    if(speed <= max_speed && speed <= speed2)
	{
//	    printf("speed:     %f\n", speed);
	    return (double) speed;
	}
    if(max_speed <= speed && max_speed <= speed2)
	{
//	    printf("max_speed: %f\n", (double) max_speed);
	    return (double) max_speed;
	}
//    printf("speed2:    %f\n", speed2);
    return (double) speed2;
}

void follow_line(int direction, double max_speed, int line_color) {
	double line[2];
	int retval = find_line_position(line_color, line);
	line_speed_calculation(direction, max_speed, retval, line);
}

void line_speed_calculation(int direction, double max_speed, int situation, double line[2]) {
	in.speed_left->data[0] = max_speed;
	in.speed_right->data[0] = max_speed;
	switch(situation) {
		case LINE_SINGLE:
			//Same action for all, just follow the line
			printf("line: %f\n", line[0]);
			if(line[0] > 3.5) {
				in.speed_left->data[0] *= (2-line[0]/3.5);
			} else {
				in.speed_right->data[0] *= line[0]/3.5;
			}
			break;
		case LINE_DOUBLE:
			//Straight have nothing to do here
			if(direction != GO_STRAIGHT) {
				int tmp = direction == GO_RIGHT ? 0 : 1;
				printf("line: %f\n", line[tmp]);
				if(line[tmp] > 3.5) {
					in.speed_left->data[0] *= (2-line[tmp]/3.5);
				} else {
					in.speed_right->data[0] *= line[tmp]/3.5;
				}
			}
			break;
		case LINE_LEFT:
			//Only left have something to do here
			if(direction == GO_LEFT) {
				printf("line: %f\n", line[0]);
				in.speed_left->data[0] = 0;
			}
			break;
		case LINE_RIGHT:
			//Only right have something to do here
			if(direction == GO_RIGHT) {
				printf("line: %f\n", line[0]);
				in.speed_right->data[0] = 0;
			}
			break;
		case LINE_CROSS:
			//Straight have nothing to do here
			if(direction == GO_RIGHT) {
				printf("line: %f\n", line[0]);
				in.speed_right->data[0] = 0;
			} else if(direction == GO_LEFT) {
				printf("line: %f\n", line[0]);
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

//TODO: Implement.
void octurn(int speed, double current_angle, double expected_angle) {}
