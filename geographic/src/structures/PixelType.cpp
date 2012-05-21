/* 
 * File:   PixelType.cpp
 * Author: ms6401
 * 
 * Created on April 25, 2012, 10:13 AM
 */
#include "PixelType.h"

#include <OpenCVUtils.h>

using namespace cv;
using namespace std;

namespace GEO{

const int PixelType::UNKNOWN  = 0;
const int PixelType::UInt8C1  = 1;
const int PixelType::UInt16C1 = 2;
const int PixelType::UInt32C1 = 3;
const int PixelType::Int16C1  = 4;

PixelType::PixelType(){
    pixeltype = UNKNOWN;
}

/** Parameterized Constructor
*/
PixelType::PixelType( const int& tp ){
    
    //set value
    pixeltype = tp;
}

/** 
 * Return the GDAL Datatype. Used for formatting the 
 * write image data.
 *
 * @return gdal datatype
*/
GDALDataType PixelType::get_gdal_type() const {

    if( pixeltype == UNKNOWN )
        return GDT_Unknown;

    if( pixeltype == UInt8C1 )
        return GDT_Byte;

    if( pixeltype == UInt16C1 )
        return GDT_UInt16;

    if( pixeltype == UInt32C1 )
        return GDT_UInt32;

    throw string("UNSUPPORTED FORMAT");
    return GDT_Unknown;
}

/** 
 * Get the name of the data type
 *
 * @return string of the name
*/
std::string PixelType::get_name() const {
    
    if( pixeltype == UNKNOWN )
        return "UNKNOWN";
    
    if( pixeltype == UInt8C1 )
        return "UInt8C1";

    if( pixeltype == UInt16C1 )
        return "UInt16C1";
    
    if( pixeltype == UInt32C1 )
        return "UInt32C1";

    throw string("UNSUPPORTED DATATYPE");
    return "Unknown";
}

int PixelType::convert(cv::Mat const& image, const int x, const int y, const int c) {
    
    /**  Lets start making this error-cognizant */
    if( c < 0 || c >= image.channels() )
        throw string("Error: c must be >= 0 and < than the number of image channels");

    //  CV_8UC1  to  UInt8C1
    if ( image.type() == CV_8UC1 && pixeltype == UInt8C1 )
        return (image.at<uchar > (y, x));

    // CV_8UC1   to  UInt16C1
    if ( image.type() == CV_8UC1 && pixeltype == UInt16C1 )
        return (image.at<uchar>(y,x)*8);

    // CV_16UC1  to  UInt8UC1 
    if ( image.type() == CV_16UC1 && pixeltype == UInt8C1 )
        return (image.at<short>(y,x)/8);

    //CV_16UC1  to UInt16C1 
    if ( image.type() == CV_16UC1 && pixeltype == UInt16C1 )
        return image.at<short>(y, x);



    /*if (image.type() == CV_8UC3) {
        Vec3b pix = image.at<Vec3b > (y, x);
        return ( (pix[0]*8 + pix[1]*8 + pix[2]*8) / 3.0);
    }

    if (image.type() == CV_16UC3) {
        Vec3s pix = image.at<Vec3s > (y, x);
        return ( (pix[0] + pix[1] + pix[2]) / 3.0);
    }*/

    if ( pixeltype == UNKNOWN )
        throw string("Conversion failed: unknown GDAL PixelType");

    throw string(string("Conversion failed: unknown OpenCV type : ") + opencvType2string(image.type()));
    return 0;
}

void PixelType::set( const int& tp ){

    pixeltype = tp;

}

int PixelType::get( )const{

    return pixeltype;
}

} //end of GEO namespace 
