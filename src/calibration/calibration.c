/*

#include <sys/ioctl.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "rhd.h"

void load_calibration(struct calibration * calibration) {
	FILE * file;
	int i;
	char data[512];
	file = fopen("../../Calibration/linesensors.dat", "r");
	if(file) {
		fread(data, sizeof(char), 512, file);
		/* FIXME: Need error detection and prettier code */
		calibration->ln_black[0] = atoi(strtok(file, " "));
		calibration->ln_white[0] = atoi(strtok(NULL, " "));
		calibration->ln_black[1] = atoi(strtok(NULL, " "));
		calibration->ln_white[1] = atoi(strtok(NULL, " "));
		calibration->ln_black[2] = atoi(strtok(NULL, " "));
		calibration->ln_white[2] = atoi(strtok(NULL, " "));
		calibration->ln_black[3] = atoi(strtok(NULL, " "));
		calibration->ln_white[3] = atoi(strtok(NULL, " "));
		calibration->ln_black[4] = atoi(strtok(NULL, " "));
		calibration->ln_white[4] = atoi(strtok(NULL, " "));
		calibration->ln_black[5] = atoi(strtok(NULL, " "));
		calibration->ln_white[5] = atoi(strtok(NULL, " "));
		calibration->ln_black[6] = atoi(strtok(NULL, " "));
		calibration->ln_white[6] = atoi(strtok(NULL, " "));
		calibration->ln_black[7] = atoi(strtok(NULL, " "));
		calibration->ln_white[7] = atoi(strtok(NULL, " "));
		fclose(file);
	} else {
		printf("ERROR: Could not read line sensor file (errno: %d)\n", errno);
		printf("Using standard values\n");
		for(i = 0; i < LINESENSORS_N; i++) {
			calibration->ln_black[i] = LN_DEFAULT_BLACK;
		}
		for(i = 0; i < LINESENSORS_N; i++) {
			calibration->ln_white[i] = LN_DEFAULT_WHITE;
		}
	}
	file = fopen("../../Calibration/inferredsensors.dat", "r");
	if(file) {
		fread(data, sizeof(char), 512, file);
		/* FIXME: Need error detection and prettier code */
		calibration->ir_ka[0] = atoi(strtok(file, " "));
		calibration->ir_kb[0] = atoi(strtok(NULL, " "));
		calibration->ir_ka[1] = atoi(strtok(NULL, " "));
		calibration->ir_kb[1] = atoi(strtok(NULL, " "));
		calibration->ir_ka[2] = atoi(strtok(NULL, " "));
		calibration->ir_kb[2] = atoi(strtok(NULL, " "));
		calibration->ir_ka[3] = atoi(strtok(NULL, " "));
		calibration->ir_kb[3] = atoi(strtok(NULL, " "));
		calibration->ir_ka[4] = atoi(strtok(NULL, " "));
		calibration->ir_kb[4] = atoi(strtok(NULL, " "));
	} else {
		printf("ERROR: Could not read inferred sensor file (errno: %d)\n", errno);
		printf("Using standard values\n");
		for(i = 0; i < IRSENSORS_N; i++) {
			calibration->ir_ka[i] = IR_DEFAULT_KA;
			calibration->ir_kb[i] = IR_DEFAULT_KB;
		}
	}
	file = fopen("../../Calibration/odometry.dat", "r");
	if(file) {
		fread(data, sizeof(char), 512, file);
		/* FIXME: Need error detection*/
		calibration->wheel_base = atof(strtok(file, " "));
		calibration->wheel_ratio = atof(strtok(NULL, " "));
	} else {
		printf("ERROR: Could not read odometry file (errno: %d)\n", errno);
		printf("Using standard values\n");
		calibration->wheel_base = ODO_DEFAULT_BASE;
		calibration->wheel_ratio = ODO_DEFAULT_RATIO;
	}
}

void save_calibration(struct calibration * calibration) {
	if(calibration->updated) {
		char data[512];
		int i = 0;
		int length = 0;
		FILE * file;
		file = fopen("../../Calibration/linesensors.dat", "w+");
		if(file) {
			for(i = 0; i < LINESENSORS_N; i++) {
				length += sprintf(data[length], "%d ", calibration->ln_black[i]);
				length += sprintf(data[length], "%d\n", calibration->ln_white[i]);
			}
			/* FIXME: Error detection */
			fwrite(data, sizeof(char), length, file);
			fclose(file);
			length = 0;
		} else {
			printf("ERROR: Linesensor values not saved (errno: %d)\n", errno);
		}
		file = fopen("../../Calibration/inferredsensors.dat", "w+");
		if(file) {
			for(i = 0; i < IRSENSORS_N; i++) {
				length += sprintf(data[length], "%d ", calibration->ir_ka[i]);
				length += sprintf(data[length], "%d\n", calibration->ir_kb[i]);
			}
			/* FIXME: Error detection */
			fwrite(data, sizeof(char), length, file);
			fclose(file);
			length = 0;
		} else {
			printf("ERROR: Inferred sensor values not saved (errno: %d)\n", errno);
		}
		file = fopen("../../Calibration/odometry.dat", "w+");
		if(file) {
			length += sprintf(data[length], "%f ", calibration->wheel_base);
			length += sprintf(data[length], "%f\n", calibration->wheel_ratio);
			/* FIXME: Error detection */
			fwrite(data, sizeof(char), length, file);
			fclose(file);
			length = 0;
		} else {
			printf("ERROR: Odometry values not saved (errno: %d)\n", errno);
		}
	}
}

void empty_stdin() {
	int arg = 0;
	ioctl(0, FIONREAD, &arg);
	while(arg) {
		getc(stdin);
		ioctl(0, FIONREAD, &arg);
	}
}

void wait_for_enter() {
	int arg = 0;
	while(!arg) {
		ioctl(0, FIONREAD, &arg);
	}
}

void wait_for_enter(char * message) {
	prinf(message);
	wait_for_enter();
	empty_stdin();
}

void calibrate_linesensors(struct calibration * calibration) {
	int i = 0;
	int j = 0;
	empty_stdin();
	wait_for_enter("Place a black paper under the line sensors, and press enter\n");
	for(j = 0; j < LINESENSORS_N; j++) {
		rhdSync();
		for(i = 0; i < MEASUREMENTS_N; i++) {
			calibration->ln_black[j] += read_linesensor_raw(j);
		}
		calibration->ln_black[j] /= MEASUREMENTS_N;
	}
	empty_stdin();
	wait_for_enter("Place a white paper under the line sensors, and press enter\n");
	for(j = 0; j < LINESENSORS_N; j++) {
		rhdSync();
                for(i = 0; i < MEASUREMENTS_N; i++) {
                        calibration->ln_white[j] += read_linesensor_raw(j);
                }
                calibration->ln_white[j] /= MEASUREMENTS_N;
        }
	calibration->updated = 1;
}

void calibrate_irsensors(struct calibration * calibration) {
	char readings[4096];
	int i = 0;
	int j = 0;
	int length = 0;
	FILE * file;
	struct timespec ts;
	ts.tv_sec = 0;
	ts.tv_nsec = 200000000;
	empty_stdin();
	wait_for_enter("Place an obstacle 15 centimeters away from each of the inferred sensors\n");
	for(i = 0; i < 9; i++) {
		for(j = 0; j < MEASUREMENTS_N; j++) {
			rhdSync();
			length += sprintf(readings[length], "%d %d %d %d %d\n", read_irsensor_raw(0), read_irsensor_raw(1), read_irsensor_raw(2), read_irsensor_raw(3), read_irsensor_raw(4));
		}
		if(i != 9) {
			empty_stdin();
			wait_for_enter("Move the obstacles five centimeters further away from the sensors\n");
		}
	}
	file = fopen("calibration_ir.dat", "w+");
	if(file) {
		/* FIXME: Need error detection and prettier code */
		fwrite(readings, sizeof(char), length, file);
		fclose(file);
		enpty_stdin();
		printf("Enter in Ka and Kb values for all the sensors supplied by Matlab:\n");
		wait_for_enter();
		length = read(stdin, readings, 512);
		calibration->ir_ka[0] = atof(strtok(readings, " "));
		calibration->ir_kb[0] = atof(strtok(NULL, " "));
		calibration->ir_ka[1] = atof(strtok(NULL, " "));
		calibration->ir_kb[1] = atof(strtok(NULL, " "));
		calibration->ir_ka[2] = atof(strtok(NULL, " "));
		calibration->ir_kb[2] = atof(strtok(NULL, " "));
		calibration->ir_ka[3] = atof(strtok(NULL, " "));
		calibration->ir_kb[3] = atof(strtok(NULL, " "));
		calibration->ir_ka[4] = atof(strtok(NULL, " "));
		calibration->ir_kb[4] = atof(strtok(NULL, " "));
		calibration->updated = 1;
	} else {
		printf("ERROR: Could not save values read from the inferred sensors\n")
		printf("No calibration of the inferred sensors can be done\n");
	}
}


void calibrate_odometry(struct calibration * calibration) {
	calibration->updated = 1;
}
*/
