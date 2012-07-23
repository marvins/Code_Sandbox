#ifndef __SRC_STRUCTURES_OPTIONS_H__
#define __SRC_STRUCTURES_OPTIONS_H__


//#include <iostream>
#include <string>

#include "vector.h"
#include "Cube.h"
#include "camera.h"


class Options{

    public:

        //Constructors
        Options(  );//: shape_type(-1), rotation_x(0), rotation_y(0), rotation_z(0);
        ~Options( );

        //Glut Parameters
        int window_width;
        int window_height;
        int start_x;
        int start_y;
        std::string window_name;


        //general parameters
        int debug_mode;
        int main_window;
        bool mouse_down;

        int x_beg, y_beg;
        int x_cur, y_cur;
        vec3 tcenter;

        int rotation_x;
        int rotation_y;
        int rotation_z;

        //important structures
        Camera  camera;
        Cube    shape;
      
        GLfloat cam_lookStep;
        GLfloat cam_moveStep;
        unsigned int cam_timerStep;


};


#endif
