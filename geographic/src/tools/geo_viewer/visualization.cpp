#include "visualization.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
//#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

/* The main drawing function. */
void DrawGLScene()
{

   glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);	// Clear The Screen And The Depth Buffer
   glLoadIdentity();				// Reset The View


   //change camera perspective
   gluLookAt(options.camera.eye.x,
           options.camera.eye.y,
           options.camera.eye.z,  
           options.camera.at.x,
           options.camera.at.y,
           options.camera.at.z,  
           options.camera.up.x,
           options.camera.up.y,
           options.camera.up.z);

   options.shape.draw_shape(); 


   // swap the buffers to display, since double buffering is used.
   glutSwapBuffers();
}

