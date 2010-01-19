
#include "../calibration/calibration.h"
#include "infrared.h"
#include "../main.h"

int read_irsensor_raw(int no) {
	return out.ir_sensor->data[no];
}

double read_irsensor_distance(int no) {
	return calibration.ir_ka[no] / ((double) read_irsensor_raw(no) - calibration.ir_kb[no]);
}

int is_closer_than(int no, double dist) {
	return (calibration.ir_ka[no] / dist) + calibration.ir_kb[no] < read_irsensor_raw(no) ? 1 : 0;
}
