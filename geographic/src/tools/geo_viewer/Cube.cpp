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


DEMPoint::DEMPoint( ){}
DEMPoint::DEMPoint( int a, int b, int l ){
    x = a;
    y = b;
    z = l;
    c = 0;
}
DEMPoint::DEMPoint( int a, int b, int l, double col ){
    x = a;
    y = b;
    z = l;
    c = col;
}

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

    glBegin( GL_TRIANGLES);
    {
        for( size_t idx=0; idx<indeces.size(); idx++ ){

            int i = indeces[idx];
            if( idx % 6 == 0 )
                glColor3f( elevation_info[i].c, elevation_info[i].c, elevation_info[i].c);
            glVertex3f( elevation_info[i].x, elevation_info[i].y, elevation_info[i].z);

        }
    
    }
    glEnd();
    /*
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
     */
}

size_t Cube::num_vertices( )const{ return 8; }

ostream& Cube::print_shape( ostream& ostr ){

    cout << "Cube Vertices" << endl;
    for(size_t i=0; i<num_vertices(); i++)
        cout << data[i] << endl;
    return ostr;
}

void Cube::set_structure( cv::Mat const& dted_image, cv::Point2f const& ul, cv::Point2f const& br ){

    cout << "Upper Left: " << ul << endl;
    cout << "Lower Right: " << br << endl;
    
    center = cv::Point2f( (br.x+ul.x)/2.0, (br.y+ul.y)/2.0);

    if( dted_image.type() != CV_8UC1 ) throw string("ERROR: Wrong type");

    int startx = 0, wx = dted_image.cols-1;
    int starty = 0, wy = dted_image.rows-1;

    
    //create the 3D structure
    for( int i=startx; i<wx; i++ )     //dted_image.cols-1; i++)
        for( int j=starty; j<wy; j++){ //dted_image.rows-1; j++){
            
            elevation_info.push_back( DEMPoint( i+0, j+0, 0, 1.8*dted_image.at<uchar>(i,j)/255.0));
            indeces.push_back( (j-starty)*wx + (i-startx) + 0);
            indeces.push_back( (j-starty)*wx + (i-startx) + 1);
            indeces.push_back( (j-starty)*wx + (i-startx) + wx + 1);
            indeces.push_back( (j-starty)*wx + (i-startx) + 0);
            indeces.push_back( (j-starty)*wx + (i-startx) + wx);
            indeces.push_back( (j-starty)*wx + (i-startx) + wx + 1);
        }
    /*
    elevation_info.push_back( DEMPoint( 0, 0, 0));
    elevation_info.push_back( DEMPoint( 1, 0, 0));
    elevation_info.push_back( DEMPoint( 1, 1, 0));
    elevation_info.push_back( DEMPoint( 0, 1, 0));
    indeces.push_back( 0 );
    indeces.push_back( 1 );
    indeces.push_back( 2 );
    indeces.push_back( 0 );
    indeces.push_back( 3 );
    indeces.push_back( 2 );
    */
}

cv::Point2f Cube::get_center()const{
    return center;
}
