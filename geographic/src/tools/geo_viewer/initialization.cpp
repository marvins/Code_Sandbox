#include "initialization.h"

void init( Parameters& options )
{
   //instanciate Shader Program
   const char vshader[] = "data/shaders/vshader.glsl";
   const char fshader[] = "data/shaders/fshader.glsl";
   options.program = InitShader( vshader, fshader);
   glUseProgram( options.program );

   glEnable( GL_DEPTH_TEST );

   //set background color
   glClearColor( 0.5, 0.5, 0.5, 1.0 ); 
}
