#ifndef _calibration_h
#define _calibration_h

#include "../main.h"

#define MEASUREMENTS_N 100

#define LN_DEFAULT_WHITE 90
#define LN_DEFAULT_BLACK 30
#define IR_DEFAULT_KA 16
#define IR_DEFAULT_KB 76
#define ODO_DEFAULT_BASE 0.26
#define ODO_DEFAULT_RATIO 1

struct calibration {
	double wheel_base;
	double wheel_ratio;
	int ls_white[LINESENSOR_N];
	int ls_black[LINESENSOR_N];
	int ir_ka[IRSENSOR_N];
	int ir_kb[IRSENSOR_N];
	int updated;
}

void load_calibration(struct calibration * calibration);
void save_calibration(struct calibration * calibration);
void empty_stdin();
void wait_for_enter();
void wait_for_enter(char * message);
void calibrate_linesensors(struct calibration * calibration);
void calibrate_irsensors(struct calibration * calibration);
void calibrate_odometry(struct calibration * calibration);


#endif
