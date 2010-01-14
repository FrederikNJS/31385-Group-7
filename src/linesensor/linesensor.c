#include "../calibration/calibration.h"
#include "linesensor.h"
#include "main.h"

int read_linesensor_raw(int no) {
	extern output out;
	return out.linesensor->data[no];
}

int read_linesensor_calibrated(int no, struct calibration * calibration) {
	int tmp = (100 * (read_linesensor_raw(no) - calibration->ls_black[no])) / (calibration->ls_white[no] - calibration->ls_black[no]);
	if(tmp > 100) {
		tmp = 100;
	} else if(tmp < 0) {
		tmp = 0;
	}
	return tmp;
}
