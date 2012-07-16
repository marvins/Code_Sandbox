#include <ctime>
#include <iostream>

#include <GL/glew.h>
#include <GL/glut.h>

#include "geo_viewer/configuration.h"
#include "geo_viewer/display.h"
#include "geo_viewer/keyboard.h"
#include "geo_viewer/MapObject.h"
#include "geo_viewer/options.hpp"
#include "geo_viewer/shader_processing.h"
#include "geo_viewer/timer.h"

using namespace std;

Options options;

/**
 *  Initialize Important GLUT and GLEW Variables
 */
void init( int argc, char* argv[] ){

    /*********************************************/
    /*                 GLUT Setup                */
    /*********************************************/
    // glut initialization
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize(     options_GLUT_WIN_X ,  options_GLUT_WIN_Y );
    glutInitWindowPosition( options_GLUT_INIT_X,  options_GLUT_INIT_Y );
    glutCreateWindow(       options_GLUT_WINDOW_NAME.c_str() );

    // glut settings
    glutSetKeyRepeat( GLUT_KEY_REPEAT_OFF );

    // initialize the OpenGL extention wrangler
    glewExperimental = GL_TRUE;
    glewInit();

    /*********************************************/
    /*               OpenGL Setup                */
    /*********************************************/
    // enable anti-aliasing (and transparency)
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.5, 0.5, 0.5, 1.0 );

    // Load shaders and use the resulting shader program
    options.program = InitShader( "data/shaders/vshader.glsl", "data/shaders/fshader.glsl" );
    glUseProgram( options.program );

    /*********************************************/
    /*                View Setup                 */
    /*********************************************/

    // set perspective
    options.zNear =  0.25;
    options.zFar  =  1.0;
    options.fova  = 60.0;
    options.ratio = 1.0 * options_GLUT_WIN_Y / options_GLUT_WIN_X;

    // TODO this probably shouldn't be done manually (make a function somewhere)
    options.projectionMatrix = Perspective(options.fova, options.ratio, options.zNear, options.zFar);

    // set up initial camera position
    options.cam_mode = CAMERA_FREE;  
    options.camera = Camera(vec4(0.0,0.0,-1,1.0), options.cam_mode);
    options.cam_moveStep = 0.01;
    options.cam_lookStep = 0.01;

    //   options.camera.rotateHoriz(90);
    //   options.camera.rotateVert(-35);

    // set up initial light position
    options.light.m_position = vec3(0.0,-5.0,10);
    options.light_timerStep = 10;
    options.light_moveStep = 0.06;

    /*********************************************/
    /*               Model Loader                */
    /*********************************************/
    
    // load models from file
    options.test_box = new MapObject( options.program );
    
    // init buffers for models
    options.test_box->init_buffers(     options.light.m_position, 
                                        options.light.m_ambient, 
                                        options.light.m_diffuse, 
                                        options.light.m_specular );
    
    // function registration
    glutDisplayFunc( display_function );
    glutKeyboardFunc( keyboardPress );
    glutKeyboardUpFunc( keyboardUp );
    /*
       glutSpecialFunc( special_keys);
       glutSpecialUpFunc( special_Upkeys);
       glutMouseFunc( mouseEvent );
       glutMotionFunc( mouseMove );
       glutReshapeFunc( reshape );
     */

    glutTimerFunc( 17, timerHandle, 0);

}

int main( int argc, char* argv[] ){

    // Process Arguments
    srand(time(0));
    init( argc, argv );


    //parse_arguements( argc, argv, options );

    //process the gui
    glutMainLoop();

    return 0;
}
