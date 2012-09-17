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

#include <GeoImage.h>


vec3 color_relief( double elevation, double minC, double maxC ){

    double maxR = maxC;
    double minR = minC;

    vec3 outColor;
    outColor.x = ((maxR - minR)-(elevation - minR))/(maxR - minR);
    outColor.y = 1 - outColor.x;
    outColor.z = 0;

    if( outColor.x > 1 ) outColor.x = 1;
    if( outColor.y > 1 ) outColor.y = 1;
    if( outColor.z > 1 ) outColor.z = 1;
    if( outColor.x < 0 ) outColor.x = 0;
    if( outColor.y < 0 ) outColor.y = 0;
    if( outColor.z < 0 ) outColor.z = 0;
    
    return outColor;
}


DEMPoint::DEMPoint( ){}
DEMPoint::DEMPoint( int a, int b, int l ){
    x = a;
    y = b;
    z = l;
    val.x = 0;
    val.y = 0;
    val.z = 0;
}

DEMPoint::DEMPoint( int a, int b, int l, double col ){
    x = a;
    y = b;
    z = l;
    val.x = col;
    val.y = col;
    val.z = col;
}

DEMPoint::DEMPoint( int a, int b, int l, vec3 col ){
    x = a;
    y = b;
    z = l;
    val = col;
}

/*
   Default Constructor
 */
Cube::Cube(){ 

}


/*
   Parameterized Constructor
 */
Cube::Cube(vector<vec4>const& a ){
    set_size( a );
    throw string("ERROR: NOT IMPLEMENTED");
}
/* 
   Copy Constructor
 */
Cube::Cube(Cube const& a ){
    throw string("ERROR: NOT IMPLEMENTED");
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
            if( idx % 3 == 0 ){
                
                glColor3f( elevation_info[i].val.x, elevation_info[i].val.y, elevation_info[i].val.z);

                //vec3 A = vec3( elevation_info[i+0].x, elevation_info[i+0].y, elevation_info[i+0].z);
                //vec3 B = vec3( elevation_info[i+1].x, elevation_info[i+1].y, elevation_info[i+1].z);
                //vec3 C = vec3( elevation_info[i+2].x, elevation_info[i+2].y, elevation_info[i+2].z);
                //vec3 nor = normalize(cross( (A-B), (C-B)));
                glNormal3f( 0, 0, 1);//nor.x, nor.y, nor.z);
            }
            glVertex3f( elevation_info[i].x, elevation_info[i].y, elevation_info[i].z);

            //cout << elevation_info[i].x << ", " << elevation_info[i].y << ", " << elevation_info[i].z << endl;
            //if( idx % 3 == 0 )
            //cin.get();

            if( elevation_info[i].x < 400 || elevation_info[i].y < 400 )
                throw string("FAIL");
        }

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

void Cube::set_structure( cv::Mat const& dted_image, cv::Point2f const& ul, cv::Point2f const& br, const int& fileType ){

    //set the center of the image
    center = cv::Point2f( (br.x+ul.x)/2.0, (br.y+ul.y)/2.0);

    double minC = dted_image.at<short>(0,0);
    double maxC = dted_image.at<short>(0,0);
    for( size_t i=0; i<dted_image.cols; i++)
        for( size_t j=0; j<dted_image.rows; j++){
            if( dted_image.at<short>(j,i) < minC )
                minC = dted_image.at<short>(j,i);
            if( dted_image.at<short>(j,i) > maxC )
                maxC = dted_image.at<short>(j,i);
        }

    //make sure the data format is consistent
    if( dted_image.type() != CV_8UC1 && dted_image.type() != CV_16SC1 ) throw string("ERROR: Wrong type");

    int startx = 590, wx = 1000;//dted_image.cols-1;
    int starty = 590, wy = 1000;//dted_image.rows-1;

    double METER2DEG = 3600/111000.0;

    //create the 3D structure
    for( int j=starty; j<=(starty+wy); j++){ 
        for( int i=startx; i<=(startx+wx); i++ ){

            if( dted_image.type() == CV_8UC1 && fileType == GEO::NITF )
                elevation_info.push_back( DEMPoint( i, j, 0, vec3(
                                1.8*dted_image.at<uchar>(j,i)/255.0, 
                                1.8*dted_image.at<uchar>(j,i)/255.0,
                                1.8*dted_image.at<uchar>(j,i)/255.0)
                            ));

            else if( dted_image.type() == CV_16SC1 && fileType == GEO::DTED ){
                elevation_info.push_back( DEMPoint( i, j, dted_image.at<short>(j,i)*METER2DEG, color_relief( dted_image.at<short>(j,i), minC, maxC) ));
            }
            else
                throw string("ERROR: Wrong type");


            if( i< (startx+wx) && j< (starty+wy) ){
                indeces.push_back( (j-starty+0)*(wx+1) + (i-startx+0) );
                indeces.push_back( (j-starty+0)*(wx+1) + (i-startx+1) );
                indeces.push_back( (j-starty+1)*(wx+1) + (i-startx+1) );
                indeces.push_back( (j-starty+0)*(wx+1) + (i-startx+0) );
                indeces.push_back( (j-starty+1)*(wx+1) + (i-startx+0) );
                indeces.push_back( (j-starty+1)*(wx+1) + (i-startx+1) );
                //cout << "i:  " << i << endl;
                //cout << "j:  " << j << endl;
                //cout << "sx: " << startx << endl;
                //cout << "sy: " << starty << endl;
                //cout << "wx: " << wx << endl;
                //cout << "wy: " << wy << endl;
                //cout << "Last index: " << indeces.back() << " -> given " << i << ", " << j <<  endl;
            }
            /*
               cout << "start: " << i << ", " << j << endl;
               int len = indeces.size();
               cout << indeces[len-6] << endl;
               cout << indeces[len-5] << endl;
               cout << indeces[len-4] << endl;
               cout << indeces[len-3] << endl;
               cout << indeces[len-2] << endl;
               cout << indeces[len-1] << endl;
               cin.get();
             */
        }
    }

    //cin.get();
}

cv::Point2f Cube::get_center()const{
    return center;
}
