
#include "../calibration/calibration.h"
#include "infrared.h"
#include "../main.h"

int read_irsensor_raw(int no) {
	return out.ir_sensor->data[no];
}

double read_irsensor_distance(int no) {
	//s - b should never be zero or below. Handle it.
	double s_b = read_irsensor_raw(no) - calibration.ir_kb[no];
	if (!(s_b > 0)) s_b = 1;
	return calibration.ir_ka[no] / ((double) s_b);
}

int is_closer_than(int no, double dist) {
	
	double s_b = read_irsensor_raw(no) - calibration.ir_kb[no];
	if (!(s_b > 0)) s_b = 0;
	return (calibration.ir_ka[no] / dist)  < s_b ? 1 : 0;
}
