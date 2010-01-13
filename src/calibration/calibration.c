#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>

void load_calibration(struct calibration * calibration) {
	FILE * file;
	int i;
	file = fopen("../../Calibration/linesensors.dat", "r");
	if(file) {
		
		fclose(file);
	} else {
		for(i = 0; i < LINESENSORS_N; i++) {
			calibration->ln_black[i] = LN_DEFAULT_BLACK;
		}
		for(i = 0; i < LINESENSORS_N; i++) {
			calibration->ln_white[i] = LN_DEFAULT_WHITE;
		}
	}
}

void wait_for_enter(char * message) {
	int arg = 0;
	
	prinf(message);
	while(!arg) {
		ioctl(0, FIONREAD, &arg);
	}
	while(arg) {
		getc(stdin);
		ioctl(0, FIONREAD, &arg);
	}
}

void calibrate_linesensors(struct calibration * calibration) {
	int reading[200][LINESENSOR_N];
	int i = 0;
	int j = 0;
	wait_for_enter("Place a black paper under the line sensors, and press enter");
	for(j = 0; j < LINESENSORS_N; j++) {
		for(i = 0; i < MEASUREMENTS_N; i++) {
			calibration->ln_black[j] += read_linesensor_raw(j);
		}
		calibration->ln_black[j] /= MEASUREMENTS_N;
	}

	wait_for_enter("Place a white paper under the line sensors, and press enter");
	for(j = 0; j < LINESENSORS_N; j++) {
                for(i = 0; i < MEASUREMENTS_N; i++) {
                        calibration->ln_white[j] += read_linesensor_raw(j);
                }
                calibration->ln_white[j] /= MEASUREMENTS_N;
        }
}
