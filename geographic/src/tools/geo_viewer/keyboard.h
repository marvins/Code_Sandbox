/**
  * @file keyboard.h
  * @brief File containing keyboard processing
  * @author Marvin Smith
*/
#ifndef __SRC_OPENGL_MANAGEMENT_KEYBOARD_H__
#define __SRC_OPENGL_MANAGEMENT_KEYBOARD_H__

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

// ../math/
#include "vector.h"

// ../structures/
#include "Parameters.h"
#include "Camera.h"

// bullet physics engine

using namespace std;

/** Global Variable Container */
extern Parameters options;

/**
  * keyboard processing function 
  * @param[in] key value from handle
  * @param[in] x x value
  * @param[in] y y value
*/
void keyboardPress( unsigned char key, int x, int y );
void keyboardUp( unsigned char key, int x, int y );

#endif
