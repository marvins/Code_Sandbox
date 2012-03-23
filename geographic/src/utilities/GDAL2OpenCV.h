#ifndef __SRC_GDAL2OPENCV_H__
#define __SRC_GDAL2OPENCV_H__

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <iostream>

#include "gdal_priv.h"

using namespace cv;
using namespace std;

int gdal2opencvPixelType( const int& gdalType );

#endif
