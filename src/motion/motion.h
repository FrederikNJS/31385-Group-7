#ifndef _motion_h
#define _motion_h

#include "../mission/task.h"

void forward(int speed, double expected_distance, task_data_t * task_data);
double speed_calc(double max_speed, double current_distance, double expected_distance);

#endif
