#include <iostream>

#include <GeoImage.h>

using namespace std;

int main( int argc, char* argv[] ){

    //check for errors here
    if( argc < 2 ){
        throw string("Invalid Number of Arguments");

    //load an image into memory
    GEO::GeoImage gimg( argv[1], true );

    //output original as OpenCV Mat
    Mat original = gimg.get_image();

    //output rectified image
    Mat rectified = GEO::ORTHO::orthorectify( gimg ).get_image();

    //write images
    imwrite( "original.jpg",  original);
    imwrite( "rectified.jpg", rectified);


    return 0;
}
