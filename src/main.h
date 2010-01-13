
#ifndef _main_h
#define _main_h


#include "rhd.h"

#define ERROR 		1
#define ROBOT_PORT	24902

typedef struct
{
    symTableElement *speed_left, *speed_right, *reset_motor_right,
	*reset_motor_left;
} input;

typedef struct
{
    symTableElement *encoder_left, *encoder_right, *line_sensor, *ir_sensor;
} output;

int init(input * in, output * out);
void term(output * out);

symTableElement *getinputref(const char *sym_name, symTableElement * tab);
symTableElement *getoutputref(const char *sym_name, symTableElement * tab);

#endif
