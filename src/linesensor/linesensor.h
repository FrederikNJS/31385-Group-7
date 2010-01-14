#ifndef _linesensor_h
#define _linesensor_h

#include "../calibration/calibration.h"

#define BLACK_LINE 0
#define WHITE_LINE 1

int read_linesensor_raw(int no);
int read_linesensor_calibrated(int no, struct calibration * calibration);
int find_line_position(int line_color, struct calibration * calibration, double lineLocations[2]);

#endif
