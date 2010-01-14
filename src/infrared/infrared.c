#include "../calibration/calibration.h"
#include "infrared.h"
#include "main.h"

int read_irsensor_raw(int no) {
	extern output out;
	return out.irsensor->data[no];
}

int read_irsensor_calibrated(int no, struct calibration * calibration) {
	return calibration->ir_ka[no] / ((double) read_irsensor_raw(no) - calibration->ir_kb[no]);
}
