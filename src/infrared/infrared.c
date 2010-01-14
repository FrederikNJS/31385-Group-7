<<<<<<< HEAD
#include "../calibration/calibration.h"
=======
#include "calibration/calibration.h"
#include "infrared.h"
>>>>>>> be52a3d827ec105076802ce0f1341d7b1fe24737

int read_irsensor_raw(int no) {
	extern output out;
	return out.irsensor->data[no];
}

int read_irsensor_calibrated(int no, struct calibration * calibration) {
	return calibration->ir_ka[no] / ((double) read_irsensor_raw(no) - calibration->ir_kb[no]);
}
