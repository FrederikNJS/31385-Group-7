/**
 * Routines to convert encoder values to positions through odometry.
 * Encoder steps have to be converted to meters, and
 * roll-over has to be detected and corrected.
 * The defines below is used in the calibration of the odometry.
 */

#ifndef _odometry_h
#define _odometry_h

#include "../calibration/calibration.h"

//odometry
#define DELTA_M (M_PI * 0.065 / 2000.0)

extern struct odometry_state current_odometry;

/*Odometry state struct.
This structure contains the current state of the odometry.
It should always be updated during missions at each rhdSync,
in order to avoid it missing data.*/
struct odometry_state
{	
	//input signals
    int left_encoder, right_encoder;	// encoderticks
    // parameters
    double w;			// wheel separation
    double cr, cl;		// meters per encodertick

	//"Position" of the encoders, measured in meters.
    double right_encoder_pos, left_encoder_pos;
    // internal variables
    int left_encoder_old, right_encoder_old;

    double old_x, old_y, x, y;
    double old_angle, angle;

	double dU;
};

/*This function should always be called during missions at each rhdSync,
in order to avoid it missing data.*/
void update_odometry(struct odometry_state * current_odometry);
/*Resets odometry state. Always call this before missions, and call it
when positioning/angle data should be reset.*/
void reset_odometry(struct odometry_state * current_odometry);

#endif
