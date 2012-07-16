/**
  * @file keyboard.h
  * @brief File containing keyboard processing
  * @author Marvin Smith
*/
#ifndef __SRC_TOOLS_GEOVIEWER_KEYBOARD_H__
#define __SRC_TOOLS_GEOVIEWER_KEYBOARD_H__

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>

#include "vec.h"

#include "options.hpp"
#include "Camera.h"


using namespace std;

/** Global Variable Container */
extern Options options;

/**
  * keyboard processing function 
  * @param[in] key value from handle
  * @param[in] x x value
  * @param[in] y y value
*/
void keyboardPress( unsigned char key, int x, int y );
void keyboardUp( unsigned char key, int x, int y );
void special_keys( int key, int x, int y );
void special_Upkeys( int key, int x, int y );

#endif
