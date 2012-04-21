#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <cmath>
#include <iostream>

#include "src/structures/GeoImage.h"


using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){


    //create GeoImage
    GeoImage geoImage(argv[1], true);

    //load image
    Mat image = geoImage.get_image();

    //convert to CV_8UC1
    if( image.channels() == 3 ){
        cvtColor( image, image, CV_BGR2GRAY);
    }
    if( image.depth() != CV_8U ){
        Mat nImage;
       
        if( image.depth() == CV_16U ){
            image = image.clone() * (1.0/(pow(2.0, 8.0)));
        }
        
        image.convertTo( nImage, CV_8U );
        image = nImage.clone();
    }

    //display
    namedWindow("Original");
    imshow("Original", image);
    waitKey(0);
    
    normalize( image, image, 0, 255, CV_MINMAX);
    equalizeHist( image, image );

    imshow("Original", image);
    waitKey(0);

    return 0;
}
