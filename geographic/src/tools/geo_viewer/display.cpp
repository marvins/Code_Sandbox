#include "display.h"

void display_function( void )
{
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   //glUniform3fv( theta, 1, Theta );
   options.board->draw_shape(   options.camera.getTransform(), options.projectionMatrix, options.light.m_position);
   
   glutSwapBuffers();

}


void ReSizeGLScene(int Width, int Height)
{

   glViewport( 0, 0, Width, Height);

   GLfloat aspect = GLfloat(Width)/Height;

   options.aspect = aspect;
   options.glut_window_x = Width;
   options.glut_window_y = Height;

   //options.camera.set_aspect_ratio( aspect);
}
