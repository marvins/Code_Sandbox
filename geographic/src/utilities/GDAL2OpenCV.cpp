#include "GDAL2OpenCV.h"

int gdal2opencvPixelType( const int& gdalType ){
    
    //convert the pixel types
    if( gdalType == GDT_Byte ){
        cout << "BYTE" << endl;
        return CV_8U;
    }
    if( gdalType == GDT_UInt16 ){
        cout << "UInt16" << endl;
        return CV_16U;
    }
    if( gdalType == GDT_UInt32 ){
        cout << "UInt32" << endl;
        return CV_32S;
    }

    cout << "ERROR" << endl;
    return 0;

}

