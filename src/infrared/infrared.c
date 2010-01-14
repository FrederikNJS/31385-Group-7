
#include "../calibration/calibration.h"
#include "infrared.h"
#include "../main.h"

int read_irsensor_raw(int no) {
	extern output out;
	return out.ir_sensor->data[no];
}

double read_irsensor_distance(int no, struct calibration * calibration) {
	return calibration->ir_ka[no] / ((double) read_irsensor_raw(no) - calibration->ir_kb[no]);
}

int is_closer_than(int no, double dist, struct calibration * calibration) {
	return (calibration->ir_ka[no] / dist) + calibration->ir_kb[no] < read_irsensor_raw(no) ? 1 : 0;
}
