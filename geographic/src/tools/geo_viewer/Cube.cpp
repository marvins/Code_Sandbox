#include "Cube.h"

//OpenGL Libraries
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


/*
   Default Constructor
*/
Cube::Cube(){ 
    
    data[0] = vec4( -1, -1, -1, 1);  //front lower left  
    data[1] = vec4(  1, -1, -1, 1);  //front lower right
    data[2] = vec4(  1,  1, -1, 1);  //front upper right
    data[3] = vec4( -1,  1, -1, 1);  //front upper left
    data[4] = vec4( -1, -1,  1, 1);    
    data[5] = vec4(  1, -1,  1, 1);    
    data[6] = vec4(  1,  1,  1, 1);    
    data[7] = vec4( -1,  1,  1, 1);    
}


/*
   Parameterized Constructor
*/
Cube::Cube(vector<vec4>const& a ){
   set_size( a );
}
/* 
   Copy Constructor
*/
Cube::Cube(Cube const& a ){
   for(int i=0; i<8; i++)
      data[i] = a.data[i];
}
/* 
   Destructors
*/
Cube::~Cube( ){ }
/*
   Cube Set Size
   - Sets the size of the shape to the desired coordinates
*/
void Cube::set_size( vector<vec4>const& a ){

   for(unsigned int i=0; i<a.size(); i++)
      data[i] = a[i];
}

/* Cube Draw Shape
   - Draws the shape of the cube
 */
void Cube::draw_shape( ){
    
    glBegin( GL_QUADS );
    {
        glColor3f( 1.0f, 0.0f, 0.0f );
        glVertex3f( data[0].x, data[0].y, data[0].z ); //front lower left
        glVertex3f( data[1].x, data[1].y, data[1].z ); //front
        glVertex3f( data[2].x, data[2].y, data[2].z );
        glVertex3f( data[3].x, data[3].y, data[3].z );

        glColor3f( 0.0f, 1.0f, 0.0f );
        glVertex3f( data[4].x, data[4].y, data[4].z );
        glVertex3f( data[5].x, data[5].y, data[5].z );
        glVertex3f( data[6].x, data[6].y, data[6].z );
        glVertex3f( data[7].x, data[7].y, data[7].z );

        glColor3f( 0.0f, 0.0f, 1.0f );
        glVertex3f( data[0].x, data[0].y, data[0].z );
        glVertex3f( data[4].x, data[4].y, data[4].z );
        glVertex3f( data[5].x, data[5].y, data[5].z );
        glVertex3f( data[1].x, data[1].y, data[1].z );

        glColor3f( 1.0f, 1.0f, 0.0f );
        glVertex3f( data[3].x, data[3].y, data[3].z );
        glVertex3f( data[7].x, data[7].y, data[7].z );
        glVertex3f( data[6].x, data[6].y, data[6].z );
        glVertex3f( data[2].x, data[2].y, data[2].z );

        glColor3f( 0.0f, 1.0f, 1.0f );
        glVertex3f( data[3].x, data[3].y, data[3].z );
        glVertex3f( data[7].x, data[7].y, data[7].z );
        glVertex3f( data[4].x, data[4].y, data[4].z );
        glVertex3f( data[0].x, data[0].y, data[0].z );

        glColor3f( 1.0f, 0.0f, 1.0f );
        glVertex3f( data[2].x, data[2].y, data[2].z );
        glVertex3f( data[1].x, data[1].y, data[1].z );
        glVertex3f( data[5].x, data[5].y, data[5].z );
        glVertex3f( data[6].x, data[6].y, data[6].z );


    }
    glEnd();

}

size_t Cube::num_vertices( )const{ return 8; }

ostream& Cube::print_shape( ostream& ostr ){

    cout << "Cube Vertices" << endl;
    for(size_t i=0; i<num_vertices(); i++)
        cout << data[i] << endl;
    return ostr;
}
