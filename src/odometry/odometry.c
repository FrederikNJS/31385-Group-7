#include <math.h>
#include "odometry.h"

struct odometry_state current_odometry;

void update_odometry(struct odometry_state * p) {
    /*
     * A part of the code here and all the comments tries to follow
     * the notation presented in exercise 3 of the assignments
	 * in course 31385 at DTU, 2010, january.
     */

    int delta;

    /*Aging. */
    p->old_x = p->x;
    p->old_y = p->y;
    p->old_angle = p->angle;

    /* U_right
	 * Handle the encoder value from the right wheel.
	 */
    delta = p->right_encoder - p->right_encoder_old;
	//Overflow.
    if(delta > 0x8000)
	delta -= 0x10000;
    else if(delta < -0x8000)
	delta += 0x10000;
    p->right_encoder_old = p->right_encoder;
    double dU_right = delta * p->cr;
    p->right_encoder_pos += dU_right;

    /* U_left
	 * Handle the encoder value from the left wheel.
	 */
    delta = p->left_encoder - p->left_encoder_old;
	//Overflow.
    if(delta > 0x8000)
	delta -= 0x10000;
    else if(delta < -0x8000)
	delta += 0x10000;
    p->left_encoder_old = p->left_encoder;
    double dU_left = delta * p->cl;
    p->left_encoder_pos += dU_left;

    /*dU = (dU_right + dU_left) / 2; */
    double dU = (dU_right + dU_left) / 2;

    /*delta O(i) = (delta U(right) - delta U(left))/wheel_distance */
    double dO = (dU_right - dU_left) / WHEEL_SEPARATION;

    /*theta(i) = theta(i-1) + delta O(i);
       x(i) = x(i-1) + dU(i)*cos(theta(i));
       y(i) = y(i-1) + dU(i)*sin(theta(i)); */

    p->angle = p->old_angle + dO;

    // interval limitation, may be useful later
    //if (p->angle > M_PI*2) p->angle -= M_PI*2;
    //if (p->angle < 0) p->angle += M_PI*2;
    p->x = p->old_x + dU * cos(p->angle);
    p->y = p->old_y + dU * sin(p->angle);
}

void reset_odometry(struct odometry_state * p)
{
    p->right_encoder_pos = p->left_encoder_pos = 0.0;
    p->x = p->y = p->angle = 0.0;
    p->old_x = p->old_y = p->old_angle = 0.0;
    p->right_encoder_old = p->right_encoder;
    p->left_encoder_old = p->left_encoder;
}

struct odometry_state * get_general_odometry(void) {
	return &current_odometry;
}
