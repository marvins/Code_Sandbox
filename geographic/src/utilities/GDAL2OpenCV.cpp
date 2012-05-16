#include "GDAL2OpenCV.h"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <iostream>

#include "gdal_priv.h"

using namespace cv;
using namespace std;

int gdal2opencvPixelType( const int& gdalType ){
    
    //convert the pixel types
    if( gdalType == GDT_Byte ){
        return CV_8U;
    }
    if( gdalType == GDT_UInt16 ){
        return CV_16U;
    }
    if( gdalType == GDT_UInt32 ){
        return CV_32S;
    }
    if( gdalType == GDT_Float64 ){
        return CV_64F;
    }
    if( gdalType == GDT_Float32 ){
        return CV_32F;
    }
    if( gdalType == GDT_Int16 ){
        return CV_16S;
    }
    if( gdalType == GDT_UInt16 ){
        return CV_16U;
    }
    
    return 0;

}

int opencv2gdalPixelType( const int& opencvType ){
    
    //convert the pixel types
    if( opencvType == CV_8U )
        return GDT_Byte;
    if( opencvType == CV_16U )
        return GDT_UInt16;
    if( opencvType == CV_32S)
        return GDT_UInt32;
    
    cout << "ERROR" << endl;
    return 0;
}
