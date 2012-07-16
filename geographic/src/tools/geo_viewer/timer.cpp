/**
 * @file timer.cpp
 * Implementation of timer handler
 * @author Marvin Smith
 */
#include "timer.h"

void timerHandle( int state ){

    // timed pause length
    if( options.timed_pause > 0 )
    {
        options.timed_pause--;
        glutTimerFunc( options.world_timerStep, &timerHandle, 0);
        return;
    }

    glutPostRedisplay(); 
    glutTimerFunc( options.world_timerStep, &timerHandle, 0);
}
