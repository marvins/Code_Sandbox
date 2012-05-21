#ifndef __DATATYPES_H__
#define __DATATYPES_H__

#include <opencv2/core/core.hpp>

using namespace std;
using namespace cv;

typedef pair<Point2d,Point2d> MatchPair; // doubles

struct PanoImage
{
    Mat img; // image
    double focal; // focal length of the camera in pixels
    double cx, cy; // optical centre of the camera in pixels
    double yaw, pitch, roll; // offset in the panoramic image
};

#endif
