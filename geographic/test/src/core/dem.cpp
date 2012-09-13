#include "dem.hpp"

#include <iostream>

using namespace cv;
using namespace std;

double query_dem( Point3f const& pnt, Options& options ){
    
    //find the min and max range, assum -500, 500
    Point demPnt( pnt.x + options.image.cols/2.0, pnt.y + options.image.rows/2.0);
    
    return options.dem.at<uchar>(demPnt);

}
