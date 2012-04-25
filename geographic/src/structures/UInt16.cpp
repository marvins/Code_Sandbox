/* 
 * File:   UInt16.cpp
 * Author: ms6401
 * 
 * Created on April 25, 2012, 10:13 AM
 */



#include "UInt16.h"
#include "../utilities/OpenCVUtils.h"

using namespace cv;
using namespace std;

PixelType*& UInt16::clone() const {


    PixelType* output = new UInt16();

    return (output);
}

GDALDataType UInt16::get_gdal_type() const {
    return GDT_UInt16;
}

std::string UInt16::get_name() const {
    return "UInt16";
}

int UInt16::convert(cv::Mat const& image, const int x, const int y) {

    //for 8 to 16 bit, multiply by 8
    if (image.type() == CV_8UC1)
        return (image.at<uchar > (y, x)*8);

    if (image.type() == CV_8UC3) {
        Vec3b pix = image.at<Vec3b > (y, x);
        return ( (pix[0]*8 + pix[1]*8 + pix[2]*8) / 3.0);
    }

    //for 16 to 16, pass straight
    if (image.type() == CV_16UC1)
        return image.at<short>(y, x);
    if (image.type() == CV_16UC3) {
        Vec3s pix = image.at<Vec3s > (y, x);
        return ( (pix[0] + pix[1] + pix[2]) / 3.0);
    }

    throw string(string("Conversion failed: unknown type : ") + opencvType2string(image.type()));
    return 0;
}
