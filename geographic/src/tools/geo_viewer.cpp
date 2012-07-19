//
// Display a color cube
//
// Colors are assigned to each vertex and then the rasterizer interpolates
//   those colors across the triangles.  We us an orthographic projection
//   as the default projetion.
#include <algorithm>
#include <iomanip>

// math/
#include "geo_viewer/mat.h"
#include "geo_viewer/vector.h"

// opengl/
#include "geo_viewer/shader_processing.h"
#include "geo_viewer/display.h"
#include "geo_viewer/keyboard.h"
#include "geo_viewer/mouse.h"
#include "geo_viewer/timer.h"
#include "geo_viewer/reshape.h"

// structures/
#include "geo_viewer/Parameters.h"

using namespace std;

Parameters options;

//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{   


    // enable anti-aliasing (and transparency)
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //// Camera Setup
    // set perspective
    options.zNear = 0.25;
    options.zFar  = 50.0;
    options.fova  = 30.0;
    options.ratio = 1.0; 

    options.projectionMatrix = Perspective(options.fova, options.ratio,
            options.zNear, options.zFar);

    // set up initial camera position
    options.cam_timerStep = 10;
    options.cam_lookStep = 1.0;
    options.cam_moveStep = 0.06;

    options.camera = Camera(vec4(5.0,2.0,5.0,1.0), options.cam_mode);
    options.camera.rotateHoriz(43.8);
    options.camera.rotateVert(-14);

    // set up initial light position
    options.light_timerStep = 10;
    options.light_moveStep = 0.06;


    // Load shaders and use the resulting shader program
    options.program = InitShader( "data/shaders/vshader.glsl", "data/shaders/fshader.glsl" );
    glUseProgram( options.program );

    glEnable( GL_DEPTH_TEST );
    glClearColor( 0.5, 0.5, 0.5, 1.0 );

    //initialize Air Hockey Board

    options.board = new Object("data/models/Airhockey.obj", 0.5, options.program);
    options.board->init_buffers( options.light.m_position, options.light.m_ambient, options.light.m_diffuse, options.light.m_specular);

}

//----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    cout << "================ Camera Controls ================" << endl
        << "============ (w,a,s,d,q,e) movement =============" << endl
        << " w - Move Forward          s - Move Backwards    " << endl
        << " a - Move Left             d - Move Right        " << endl
        << " q - Move Up               e - Move Down         " << endl
        << "============== (u,h,j,k,y,i) look ===============" << endl
        << " i - Look Up               k - Look Down         " << endl
        << " j - Look Left             l - Look Right        " << endl
        << "== Twist works only if camera is in free mode ===" << endl
        << " u - Twist CCW             o - Twist CW          " << endl
        << "=================== lighting model ==============" << endl 
        << " n - move light -z axis   m - move light +z axis " << endl
        << " , - move light -x axis   . - move light +x axis " << endl
        << "=================================================" << endl << endl;

    //set up some initial window parameters
    options.glut_window_x = 700;
    options.glut_window_y = 700;

    options.glut_initial_x = 40;
    options.glut_initial_y = 5;

    options.glut_window_name = "Geo Viewer";
    srand(time(0));

    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowSize( options.glut_window_x, options.glut_window_y );
    glutInitWindowPosition(options.glut_initial_x,options.glut_initial_y);
    glutCreateWindow( options.glut_window_name.c_str() );

#ifndef __APPLE__
    glewInit();
#endif
    init();

    glutSetKeyRepeat( GLUT_KEY_REPEAT_OFF );

    glutDisplayFunc( display_function );

    // Keyboard down/up functions
    glutKeyboardFunc( keyboardPress );
    glutKeyboardUpFunc( keyboardUp );

    // Mouse func
    glutMouseFunc( mouseEvent );

    // Dragging the mouse
    glutMotionFunc( mouseMove );

    // When the screen is resized
    glutReshapeFunc( reshape );

    // timer function
    glutTimerFunc( 20, timerHandle, 0);

    glutMainLoop();
    return 0;
}
