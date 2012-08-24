#include <iostream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <GeoImage.h>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] ){

    GEO::GeoImage img( argv[1], true );

    Mat result = img.get_image();
    Mat formatted;

    //make sure we convert to 8uc1 always
    if( result.type() == CV_8UC1 || result.type() == CV_8UC3 ){
        
        //do nothing
        formatted = result.clone();
    }
    else if( result.type() == CV_16UC1 ){
        
        //create output object
        result.convertTo( formatted, CV_8UC1, 1/255.0, 0 );
    }
    else{
        cout << "unsupported type" << endl;
        return -1;
    }

    imwrite( argv[2], formatted);

    return 0;
}
