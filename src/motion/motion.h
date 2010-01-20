#ifndef _motion_h
#define _motion_h

#include "../mission/task.h"
#include "../calibration/calibration.h"

#define GO_STRAIGHT 0
#define GO_LEFT -1
#define GO_RIGHT 1

double absanglediff(double angle1, double angle2);
double absd(double angle1);
void forward(int speed, task_data_t * task_data);
void reverse(int speed, task_data_t * task_data);
void turn(int speed, double current_angle, double expected_angle, double start_angle);
void octurn(int speed, double current_angle, double expected_angle);
void off_center_turn(int speed, double current_angle, double expected_angle);
double speed_calc(double max_speed, double current_distance, double expected_distance);
void follow_line(int direction, double max_speed, int line_color);
void line_speed_calculation(int direction, double max_speed, int situation, double line[2]);

#endif
