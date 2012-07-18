/**
  * @file timer.cpp
  * Implementation of timer handler
  * @author Marvin Smith
*/
#include "timer.h"

void timerHandle( int state ){

   glutPostRedisplay(); 
   glutTimerFunc( 20, &timerHandle, 0);

}
