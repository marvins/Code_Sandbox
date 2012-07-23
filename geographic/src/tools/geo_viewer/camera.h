#ifndef __SRC_STRUCTURES_CAMERA_CAMERA_H__
#define __SRC_STRUCTURES_CAMERA_CAMERA_H__

#include <cmath>

#include "vector.h"

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glut.h>
#endif

class Camera{

   public:

        Camera( );
        Camera( vec4 const& e, vec4 const& a, vec4 const& u);

        void shift_horizontal( GLfloat const& amt );
        void shift_straight( GLfloat const& amt );
        void shift_vertical( GLfloat const& amt );

        void rotate_vertical(   GLfloat const& theta );
        void rotate_horizontal( GLfloat const& theta );

        vec4 eye;
        vec4 at;
        vec4 up;

};

#endif
