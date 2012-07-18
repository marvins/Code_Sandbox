
#include "reshape.h"

void reshape(int width, int height)
{
   glViewport(0,0,width,height);

   options.aspect = (GLfloat)width/(GLfloat)height;
   options.projectionMatrix = Perspective(options.fova, options.aspect,
         options.zNear, options.zFar);
   
   // set perspective projection in shader
   glUniformMatrix4fv( options.projection, 1, GL_TRUE, options.projectionMatrix );
   
   glutPostRedisplay();
}

