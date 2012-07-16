#ifndef __SRC_TOOLS_GEOVIEWER_OPTIONS_HPP__
#define __SRC_TOOLS_GEOVIEWER_OPTIONS_HPP__

#include "Camera.h"
#include "Light.h"
#include "MapObject.h"

class Options {
    public:
        
        ///General Parameters
        GLuint program;   /*< Reference to shaders */
        int view_mode;
        int timed_pause;

        /************************************/
        /*       Viewport Parameters        */
        /************************************/
        GLfloat fova;
        GLfloat ratio;
        GLfloat zNear;
        GLfloat zFar;

        mat4 projectionMatrix;

        /************************************/
        /*         Light Parameters         */
        /************************************/
        Light  light;      /*< Light Model Abstraction */
        unsigned int light_timerStep; 
        GLfloat light_moveStep;
        GLfloat world_timerStep;

        /************************************/
        /*        Camera Parameters         */
        /************************************/
        Camera camera;     /*< Camera Model Abstraction */ 
        CameraMode cam_mode;

        GLfloat cam_lookStep;
        GLfloat cam_moveStep;
        
        unsigned int cam_timerStep;

        /****************************/
        /*        Test Object       */
        /****************************/
        MapObject* test_box;

};

#endif
