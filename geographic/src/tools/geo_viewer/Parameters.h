/**
  * @file Paramters.h
  * Parameter container for Air Hockey
  *
  * @author Marvin Smith
  * @author Joshua Gleason
*/
#ifndef __SRC_STRUCTURES_PARAMETERS_H__
#define __SRC_STRUCTURES_PARAMETERS_H__

// ../opengl
#include "GL.h"

#include <boost/program_options.hpp>
#include <boost/shared_ptr.hpp>
#include <iostream>

// ./
#include "Object.h"
#include "Camera.h"
#include "Light.h"

// ../math/
#include "vector.h"
#include "mat.h"


using namespace std;


/**
   @class Parameters
   @brief Storage for program options

   This class stores everything for the OpenGL Program

*/
class Parameters{

   public:
      /**
        * Parameter Constructor
        * 
        * builds the parameter class and sets the mode
        * lock
      */
      Parameters();
      /**
        * Destructor
      */
      ~Parameters();
     
      /**
        * Print parameter values in order to see program configuration
      */
      void print();
      
      /**
        * construct the shapes and initialize them in OpenGL
      */
      void construct_objects( );
      
      void rotate();
   
      /** Mouse is down command */
      bool mouse_down;

      /** Anchor for shape initial is set */
      //int mouse_anchor;

      int x_beg, y_beg, x_cur, y_cur;

      GLfloat aspect;

      string filename;

      GLuint program;

      GLuint model_view;
      GLuint projection;

      point4 eye;
      point4 center;
      point4 tcenter;
      vec4 up;

      GLfloat fova;
      GLfloat ratio;
      GLfloat left;
      GLfloat right;
      GLfloat top;
      GLfloat bottom;
      GLfloat zNear;
      GLfloat zFar;

      mat4 projectionMatrix;

      CameraMode cam_mode;
      GLfloat cam_lookStep;
      GLfloat cam_moveStep;
      unsigned int cam_timerStep;

      Object* board;

      Camera camera;
      Light light;
      unsigned int light_timerStep;
      GLfloat light_moveStep;

      int glut_window_x;
      int glut_window_y;
      int glut_initial_x;
      int glut_initial_y;
      string glut_window_name;

};

#endif
