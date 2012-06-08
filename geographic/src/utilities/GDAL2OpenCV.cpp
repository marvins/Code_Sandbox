#include "GDAL2OpenCV.h"

#include "gdal_priv.h"
#include "cpl_conv.h"

#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

#include <string>

int gdal2opencvPixelDepth( const int& gdalType ){

    if( gdalType == GDT_Byte   ) return CV_8U;
    if( gdalType == GDT_UInt16 ) return CV_16U;
    if( gdalType == GDT_Int16  ) return CV_16S;
    if( gdalType == GDT_UInt32 ) return CV_32S;
    if( gdalType == GDT_Int32  ) return CV_32S;
    

    throw std::string("TYPE NOT SUPPORTED");

}
