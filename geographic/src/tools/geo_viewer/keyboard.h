#ifndef __SRC_OPENGL_MANAGEMENT_KEYBOARD_H__
#define __SRC_OPENGL_MANAGEMENT_KEYBOARD_H__

//OpenGL Libraries
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#include "Options.h"

/* ASCII code for the escape key. */
#define ESCAPE 27

extern Options options;

void keyboardPress( unsigned char key, int x, int y );
void keyboardUp( unsigned char key, int x, int y );


#endif
