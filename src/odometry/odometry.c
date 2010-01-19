#include <math.h>
#include <stdio.h>
#include "odometry.h"
#include "../main.h"
#include "../calibration/calibration.h"
#include "../motion/motion.h"

struct odometry_state current_odometry;

void update_odometry(struct odometry_state * p) {
    /*
     * A part of the code here and all the comments tries to follow
     * the notation presented in exercise 3 of the assignments
	 * in course 31385 at DTU, 2010, january.
     */

    short delta;

    p->w = calibration.wheel_ratio;
    p->cr = DELTA_M;
    p->cl = p->cr;

	printf("Values: %f,   %f\n",  calibration.wheel_base, calibration.wheel_ratio);

    /*Aging. */
    p->old_x = p->x;
    p->old_y = p->y;
    p->old_angle = p->angle;

    /* U_right
	 * Handle the encoder value from the right wheel.
	 */
	p->right_encoder = out.encoder_right->data[0];
    delta = p->right_encoder - p->right_encoder_old;
	//Overflow.
    /*if(delta > 0x8000)
	delta -= 0x8000;
    else if(delta < -0x8000)
	delta += 0x8000;*/
    p->right_encoder_old = p->right_encoder;
    double dU_right = delta * p->cr*calibration.wheel_ratio;
    p->right_encoder_pos += dU_right;

    /* U_left
	 * Handle the encoder value from the left wheel.
	 */
	p->left_encoder = out.encoder_left->data[0];
    delta = p->left_encoder - p->left_encoder_old;
	//Overflow.
    /*if(delta > 0x8000)
	delta -= 0x10000;
    else if(delta < -0x8000)
	delta += 0x10000;*/
    p->left_encoder_old = p->left_encoder;
    double dU_left = delta * p->cl/calibration.wheel_ratio;
    p->left_encoder_pos += dU_left;

    /*dU = (dU_right + dU_left) / 2; */
    double dU = absd(dU_right + dU_left) / 2;
	p->dU = dU;

    /*delta O(i) = (delta U(right) - delta U(left))/wheel_distance */
    double dO = (dU_right - dU_left) / calibration.wheel_base;

    /*theta(i) = theta(i-1) + delta O(i);
       x(i) = x(i-1) + dU(i)*cos(theta(i));
       y(i) = y(i-1) + dU(i)*sin(theta(i)); */

    p->angle = p->old_angle + dO;

    // interval limitation, may be useful later
    //if (p->angle > M_PI*2) p->angle -= M_PI*2;
    //if (p->angle < 0) p->angle += M_PI*2;
    p->x = p->old_x + dU * cos(p->angle);
    p->y = p->old_y + dU * sin(p->angle);

	printf("state_odo: x:%f, y:%f, dU:%f, dO%f\n", p->x, p->y, dU, dO);
}

void reset_odometry(struct odometry_state * p)
{
    p->right_encoder_pos = p->left_encoder_pos = 0.0;
    p->x = p->y = p->angle = 0.0;
    p->old_x = p->old_y = p->old_angle = 0.0;
    p->right_encoder_old = p->right_encoder = out.encoder_right->data[0];
    p->left_encoder_old = p->left_encoder = out.encoder_left->data[0];
    p->dU = 0;

    p->w = calibration.wheel_ratio;
    p->cr = DELTA_M;
    p->cl = p->cr;

	printf("reset, state_odo: x:%f, y:%f, dU:%f\n", p->x, p->y, p->dU);
}
