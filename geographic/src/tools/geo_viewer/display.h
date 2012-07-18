#ifndef __SRC_OPENGL_MANAGEMENT_DISPLAY_H__
#define __SRC_OPENGL_MANAGEMENT_DISPLAY_H__

#include "GL.h"

#include "Parameters.h"

#include <sstream>
#include <string>

using namespace std;

extern Parameters options;

void display_function( );

void init_menu();

void ReSizeGLScene(int Width, int Height);

#endif
