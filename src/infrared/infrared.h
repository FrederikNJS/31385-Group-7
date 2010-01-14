#ifndef _infrared_h
#define _infrared_h

#include "../main.h"
#include "../calibration/calibration.h"


int read_irsensor_raw(int no);
double read_irsensor_distance(int no, struct calibration * calibration);
int is_closer_than(int no, double dist, struct calibration * calibration);

#endif
