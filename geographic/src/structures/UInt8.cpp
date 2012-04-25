/* 
 * File:   UInt8.cpp
 * Author: ms6401
 * 
 * Created on April 25, 2012, 1:36 PM
 */

#include "UInt8.h"
#include "../utilities/OpenCVUtils.h"

using namespace cv;
using namespace std;

PixelType*& UInt8::clone() const {


    PixelType* output = new UInt8();

    return (output);
}

GDALDataType UInt8::get_gdal_type() const {
    return GDT_Byte;
}

std::string UInt8::get_name() const {
    return "UInt8";
}

int UInt8::convert(cv::Mat const& image, const int x, const int y) {

    //for 8 to 8 bit, don't touch
    if (image.type() == CV_8UC1)
        return (image.at<uchar > (y, x));

    if (image.type() == CV_8UC3) {
        Vec3b pix = image.at<Vec3b > (y, x);
        return ( (pix[0] + pix[1] + pix[2]) / 3.0);
    }

    //for 16 to 8, divide by 8
    if (image.type() == CV_16UC1)
        return (image.at<short>(y, x)/8);
    if (image.type() == CV_16UC3) {
        Vec3s pix = image.at<Vec3s > (y, x);
        return ( (pix[0]/8.0 + pix[1]/8.0 + pix[2]/8.0) / 3.0);
    }

    throw string(string("Conversion failed: unknown type : ") + opencvType2string(image.type()));
    return 0;
}
