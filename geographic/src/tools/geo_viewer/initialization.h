#ifndef __SRC_OPENGL_MANAGEMENT_INITIALIZATION_H__
#define __SRC_OPENGL_MANAGEMENT_INITIALIZATION_H__

//OpenGL Libraries
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

void InitGL(int Width, int Height);	        // We call this right after our OpenGL window is created.

void ReSizeGLScene(int Width, int Height);

#endif
