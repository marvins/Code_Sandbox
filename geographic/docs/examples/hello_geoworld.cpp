#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <GeoImage.h>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){


    cout << "usage: ./" << argv[0] << "   <Image Name>" << endl;
    if( argc < 2 )
        return 0;
    
    //create a GeoImage using the input filename
    GEO::GeoImage geoimg( argv[1], true );
   
    //modify some header metadata tag
    geoimg.modify_header_metadata( "NITF_ISORCE", "Some random header value", 1); 
    
    //grab the opencv image
    Mat image = geoimg.get_image();

    //invert the image
    Mat output = Scalar(255) - image;

    //show the image as well as the inverted image
    namedWindow("IMG");
    imshow("IMG", image);
    waitKey(0);
    imshow("IMG", output);
    waitKey(0);
    
    //set the internal image to the output
    geoimg.set_image( output );

    //write the image to file
    geoimg.write_image();

    return 0;
}
