#include "mouse.h"

/**
  * window2world
  * 
  * A very simple function which takes the window parameters from 
  * glut and returns the world coordinate positions
  *
  * @param[in] x Glut x coordinate
  * @param[in] y Glut y coordinate
  * @return world coordinates
*/
static vec3 window2world( const int x, const int y ){

   /************************************************************/
   /*   Convert the mouse coordinates into World Coordinates   */
   /************************************************************/
   //retrieve the viewport data
   GLint viewport[4];
   glGetIntegerv(GL_VIEWPORT, viewport );

   //retrieve the modelview
   GLdouble modelview[16];
   glGetDoublev(GL_MODELVIEW_MATRIX, modelview);

   //retieve the projection matrix
   GLdouble projection[16];
   glGetDoublev(GL_PROJECTION_MATRIX, projection);

   //reverse the bit of the mouse y to get accurate location
   GLdouble wx = x;
   GLdouble wy = viewport[3] - y;
   GLdouble wz;
   glReadPixels( wx, wy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &wz);

   //compute the world coordinates
   GLdouble pX, pY, pZ;
   gluUnProject( wx, wy, wz, modelview, projection, viewport, &pX, &pY, &pZ);

   return vec3( pX, pY, -0.5);
}

/**
 * function to handle mouse presses
 * @param button  the button pressed
 * @param state  the type of press (up or down)
 * @param x  the x coordinate of the press
 * @param y  the y coordinate of the press
 */
void mouseEvent(int button, int state, int x, int y) 
{

   //Get the current mouse position in world coordinates
   vec3 world_points = window2world( x, y);
  
   if( state == 0 && options.mouse_down == false ){
      options.mouse_down = true;
      options.x_beg = x;
      options.y_beg = y;
      options.tcenter = options.center;
   }
   if( state == 1 && options.mouse_down == true ){
      options.mouse_down = false;
   }
}

/**
 * function to handle mouse movement
 * @param x  the x coordinate of the mouse
 * @param y  the y coordinate of the mouse
 */
void mouseMove(int x, int y)
{
   //Get the current mouse position in world coordinates
   vec4 world_points = window2world( x, y);

   if( options.mouse_down == true ){
      options.x_cur = x;
      options.y_cur = y;

      options.rotate( );
   }
}

