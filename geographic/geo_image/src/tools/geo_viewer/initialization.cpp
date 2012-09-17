#include "initialization.h"

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
   glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		// This Will Clear The Background Color To Black
   glClearDepth(1.0);				// Enables Clearing Of The Depth Buffer
   glDepthFunc(GL_LESS);			        // The Type Of Depth Test To Do
   glEnable(GL_DEPTH_TEST);		        // Enables Depth Testing
   glShadeModel(GL_SMOOTH);			// Enables Smooth Color Shading

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();				// Reset The Projection Matrix

   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window

   glMatrixMode(GL_MODELVIEW);
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
   if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
      Height=1;

   glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();

   gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
   glMatrixMode(GL_MODELVIEW);
}
