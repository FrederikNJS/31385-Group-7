#ifndef _calibration_h
#define _calibration_h

#include "../main.h"

#define MEASUREMENTS_N 100

#define LN_DEFAULT_WHITE 90
#define LN_DEFAULT_BLACK 30
#define IR_DEFAULT_KA 16.0
#define IR_DEFAULT_KB 76.0
#define ODO_DEFAULT_BASE 0.26
#define ODO_DEFAULT_RATIO 1

typedef struct {
	double wheel_base;
	double wheel_ratio;
	int ls_white[LINESENSOR_N];
	int ls_black[LINESENSOR_N];
	double ir_ka[IRSENSOR_N];
	double ir_kb[IRSENSOR_N];
	int updated;
} calibration;

extern calibration calibration;

void load_calibration();
void save_calibration();
void empty_stdin();
void wait_for_enter();
void wait_for_enter_message(char * message);
void calibrate_linesensors();
void calibrate_irsensors();
void calibrate_odometry();


#endif
