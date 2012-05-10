/* 
 * File:   PixelType.cpp
 * Author: ms6401
 * 
 * Created on April 25, 2012, 10:13 AM
 */

#include "gdal_priv.h"
#include "cpl_conv.h"

#include "PixelType.h"
#include "../utilities/OpenCVUtils.h"

using namespace cv;
using namespace std;

GDALDataType PixelType::get_gdal_type() const {

    if( pixeltype == 0 )
        throw string("No pixeltype set");

    if( pixeltype == UInt8C1 )
        return GDT_Byte;

    if( pixeltype == UInt16C1 )
        return GDT_UInt16;

    return GDT_Unknown;
}

std::string PixelType::get_name() const {
    
    if( pixeltype == 0 )
        throw string("No pixeltype set");
    
    if( pixeltype == UInt8C1 )
        return "UInt8";

    if( pixeltype == UInt16C1 )
        return "UInt16";

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
