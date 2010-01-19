#include "../calibration/calibration.h"
#include "linesensor.h"
#include "../main.h"

int read_linesensor_raw(int no) {
	return out.line_sensor->data[no];
}

int read_linesensor_calibrated(int no) {
	int tmp = (100 * (read_linesensor_raw(no) - calibration.ls_black[no])) / (calibration.ls_white[no] - calibration.ls_black[no]);
	if(tmp > 100) {
		tmp = 100;
	} else if(tmp < 0) {
		tmp = 0;
	}
	return tmp;
}

int find_line_position(int line_color, double lineLocations[2]) {
	int i = 0;
	int sensor[LINESENSOR_N];
	int start1 = 0;
	int end1 = 0;
	int start2 = 0;
	int end2 = 0;
	double dividend = 0;
	double divisor = 0;
	int value1 = 101;
	int value2 = 101;
	int lowest1 = 0;
	int lowest2 = 0;
	int low = 30;
	int situation = 0;
	int temp = 0;
	//Detects the two lowest values and positions in the sensor data
	for(i = 0; i < LINESENSOR_N; i++) {
		if(line_color) {
			sensor[i] = 100 - read_linesensor_calibrated(i);
		} else {
			sensor[i] = read_linesensor_calibrated(i);
		}
		temp = sensor[i];
		if(temp < value1) {
			lowest1 = i;
			value1 = temp;
		} else if(temp < value2) {
			//The second lowest value can not be right next to the first one
			//since both of them is probably detecting the same line
			if(i != lowest1 + 1) {
				lowest2 = i;
				value2 = temp;
			}
		}
		//Detects when it enters and leaves a row of sensors which is below the threshold
		//It can only detect up to two lines
		if(temp < low) {
			if(start1 == -1) {
				start1 = i;
			} else if(start2 == -1 && end1 != -1) {
				start2 = i;
			}
		} else {
			if(end1 == -1 && start1 != -1) {
				end1 = i-1;
			} else if(end2 == -1 && start2 != -1) {
				end2 = i-1;
			}
		}
	}
	//If a streak of low sensors were started but not ended before all the sensors had been read
	//it will end them with the last sensor.
	if(start1 != -1 && end1 == -1) {
		end1 = LINESENSOR_N - 1;
	}
	if(start2 != -1 && end2 == -1) {
		end2 = LINESENSOR_N - 1;
	}
	//If (almost) all the sensors are within the first low sensor data range
	//it have detected a cross road
	if(start1 <= 0 && end1 >= LINESENSOR_N) {
		situation = LINE_CROSS;
	} else if(start1 == 0 && end1 >= (LINESENSOR_N / 2)) {
		//If all the sensors from left to the middle is within the first streak
		//it have detected a left turn
		situation = LINE_LEFT;
	} else if(start1 < (LINESENSOR_N / 2) && end1 == (LINESENSOR_N - 1)) {
		//If all the sensors from the middle to the right is within the first streak
		//it have detected a right turn
		situation = LINE_RIGHT;
	} else if(start2 == -1) {
		//If none of the above situations have occurred, and the second streak is not started
		//It have simply detected a single line
		situation = LINE_SINGLE;
		//For calculating the location of the line it will use weighed mean, but
		//only from right before to right next the lowest sensor (with the correct weight according to their position)
		i = lowest1 - 1;
		temp = lowest1 + 1;

		//If the lowest value were detected at sensor 0 or 7, it will use a sensor which is outside range
		//So if the lowest is sensor 0, it will not try to get data from sensor -1
		//And if the lowest is sensor 7, it will not try to get data from sensor 8
		if(i == -1) {
			i++;
		}
		if(temp == LINESENSOR_N) {
			temp--;
		}
		for(; i < temp; i++) {
			dividend +=  (i + 1) * (100 - sensor[i]);
			divisor += sensor[i];
		}
		//Prevents divide by 0
		if(!divisor) {
			divisor = 1;
		}
		lineLocations[0] = dividend / divisor;
	} else if(end2 != -1) { //two roads
		//If both streaks of low sensors is set, two lines have been detected
		situation = LINE_DOUBLE;
		//Finds the lowest sensor in the first streak
		for(i = start1; i <= end1; i++) {
			temp = sensor[i];
			if(temp < value1) {
				lowest1 = i;
				value1 = temp;
			}
		}
		//Finds the lowest sensor in the second streak
		for(i = start2; i <= end2; i++) {
			temp = sensor[i];
			if(temp < value2) {
				lowest2 = i;
				value2 = temp;
			}
		}
		//Weighed mean line with a single line
		i = lowest1 - 1;
		temp = lowest1 + 1;
		if(i == -1) {
			i++;
		}
		if(temp == LINESENSOR_N) {
			temp--;
		}
		for(; i < temp; i++) {
			dividend += (100 - sensor[i]) * (i + 1);
			divisor += sensor[i];
		}
		if(!divisor) {
			divisor = 1;
		}
		lineLocations[0] = dividend/divisor;
		i = lowest2 - 1;
		temp = lowest2 + 1;
		if(i == -1) {
			i++;
		}
		if(temp == LINESENSOR_N) {
			temp--;
		}
		for(; i < temp; i++) {
			dividend += (100 - sensor[i]) * (i+1);
			divisor += sensor[i];
		}
		if(!divisor) {
			divisor = 1;
		}
		lineLocations[1] = dividend/divisor;
	} else {
		situation = LINE_NONE;
	}
	return situation;
}
