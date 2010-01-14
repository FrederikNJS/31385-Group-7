#ifndef _infrared_h
#define _infrared_h

#include "../main.h"
#include "../calibration/calibration.h"


int read_irsensor_raw(int no);
int read_irsensor_calibrated(int no, struct calibration * calibration);

#endif
