
#ifndef _mission_h
#define _mission_h

#include "../main.h"
enum mission_states
{ M_START, M_FINISHED, M_SQUARE, M_DISTANCE_TO_BOX, M_MOVE_OBSTACLE,
    M_FIND_GHOST_GATE, M_WALL_HUGGING, M_WHITE_IS_THE_NEW_BLACK,
    M_HARDCORE_PARKING_ACTION
};

void mission(int start_state, int speed);

#endif
