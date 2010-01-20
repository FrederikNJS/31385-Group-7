#ifndef _linesensor_h
#define _linesensor_h

#include "../calibration/calibration.h"

#define BLACK_LINE 0
#define WHITE_LINE 1

#define LINE_ANY    -4
#define LINE_CROSS	-3
#define LINE_RIGHT	-2
#define LINE_LEFT	-1
#define LINE_NONE	0
#define LINE_SINGLE	1
#define LINE_DOUBLE	2

int read_linesensor_raw(int no);
int read_linesensor_calibrated(int no);
int find_line_position(int line_color, double lineLocations[2]);

#endif
