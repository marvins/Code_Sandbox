#include <GeoImage.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

int main( int argc, char* argv[] ){

    GEO::GeoImage img( argv[1], true );

    Mat out = img.get_image();

    imwrite( argv[2], out);

    return 0;
}
