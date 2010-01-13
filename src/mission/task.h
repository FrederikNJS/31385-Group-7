#include "main.h"

enum
{ FORWARD, TURN, REVERSE, WAIT, FOLLOW, FOLLOW_RIGHT, FOLLOW_STRAIGHT,
    FOLLOW_LEFT, STOP, FINISHED
} task;

enum
{ TIME = 1, ODOMETRY = 1 << 1, LINE = 1 << 2, INFRARED = 1 << 3 } trigger;

typedef struct
{
    int speed;			//sets the maxspeed
    char distance_enable;	//1 of odometry should be checked, 0 if not
    double distance;		//in centimeters
    double ir_distance[];	//in centimeters
    double time;		//in seconds
    int line;			//special case for the line sensor
} task_parameters;

int task(int task_id, int trigger, input * in, output * out);
