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
        cout << "GDT_BYTE" << endl;
        return CV_8U;
    }
    if( gdalType == GDT_UInt32 ){
        cout << "GDT_UInt32" << endl;
        return CV_32S;
    }
    if( gdalType == GDT_Float64 ){
        cout << "GDT_Float64" << endl;
        return CV_64F;
    }
    if( gdalType == GDT_Float32 ){
        cout << "GDT_Float32" << endl;
        return CV_32F;
    }
    if( gdalType == GDT_Int16 ){
        cout << "GDT_Int16" << endl;
        return CV_16S;
    }
    if( gdalType == GDT_UInt16 ){
        cout << "GDT_UInt16" << endl;
        return CV_16U;
    }
    
    cout << "UNKNOWN" << endl;
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
