#ifndef _linesensor_h
#define _linesensor_h

#include "../calibration/calibration.h"

int read_linesensor_raw(int no);
int read_linesensor_calibrated(int no, struct calibration * calibration);

#endif
