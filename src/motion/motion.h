#ifndef _motion_h
#define _motion_h

void forward(int speed, double current_distance, double expected_distance);
double speed_calc(double max_speed, double current_distance,
		  double expected_distance);

#endif
