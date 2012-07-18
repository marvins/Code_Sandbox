#ifndef __LOCAL_GL_INCLUDE_FILE_H__
#define __LOCAL_GL_INCLUDE_FILE_H__

#define BUFFER_OFFSET( offset )  ((GLvoid*)offset) 

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#endif // __LOCAL_GL_INCLUDE_FILE_H__
