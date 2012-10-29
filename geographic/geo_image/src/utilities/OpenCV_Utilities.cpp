#include "OpenCV_Utilities.hpp"

#include <iostream>

using namespace cv;
using namespace std;

namespace GEO{

/**
 * Convert Type 2 Depth 
*/
int cvType2Depth( const int& type ){
    
    if( type == CV_8UC1 ) return CV_8U;
    if( type == CV_8UC2 ) return CV_8U;
    if( type == CV_8UC3 ) return CV_8U;
    
    if( type == CV_16UC1 ) return CV_16U;
    if( type == CV_16UC2 ) return CV_16U;
    if( type == CV_16UC3 ) return CV_16U;
    
    if( type == CV_16SC1 ) return CV_16S;
    if( type == CV_16SC2 ) return CV_16S;
    if( type == CV_16SC3 ) return CV_16S;
    
    if( type == CV_32SC1 ) return CV_32S;
    if( type == CV_32SC2 ) return CV_32S;
    if( type == CV_32SC3 ) return CV_32S;
    
    if( type == CV_32FC1 ) return CV_32F;
    if( type == CV_32FC2 ) return CV_32F;
    if( type == CV_32FC3 ) return CV_32F;
    
    if( type == CV_64FC1 ) return CV_64F;
    if( type == CV_64FC2 ) return CV_64F;
    if( type == CV_64FC3 ) return CV_64F;

    return -1;
}


/**
 * Convert Depth and Channels into Type
*/
int cvDepthChannel2Type( const int Depth, const int Channels ){

    if( Depth == CV_8U  && Channels == 3 )  return CV_8UC3;
    if( Depth == CV_8U  && Channels == 2 )  return CV_8UC2;
    if( Depth == CV_8U  && Channels == 1 )  return CV_8UC1;
    if( Depth == CV_16U && Channels == 3 ) return CV_16UC3;
    if( Depth == CV_16U && Channels == 2 ) return CV_16UC2;
    if( Depth == CV_16U && Channels == 1 ) return CV_16UC1;
    if( Depth == CV_16S && Channels == 3 ) return CV_16SC3;
    if( Depth == CV_16S && Channels == 2 ) return CV_16SC2;
    if( Depth == CV_16S && Channels == 1 ) return CV_16SC1;
    if( Depth == CV_32S && Channels == 3 ) return CV_32SC3;
    if( Depth == CV_32S && Channels == 2 ) return CV_32SC2;
    if( Depth == CV_32S && Channels == 1 ) return CV_32SC1;

    throw string("Error: combo not supported");

    return 0;
}

/**
 * Print the opencv type. 
*/
std::string opencvType2string( const int& type ){
    if( type == CV_8UC1 )  return "CV_8UC1";
    if( type == CV_8UC2 )  return "CV_8UC2";
    if( type == CV_8UC3 )  return "CV_8UC3";
    
    if( type == CV_16UC1 ) return "CV_16UC1";
    if( type == CV_16UC2 ) return "CV_16UC2";
    if( type == CV_16UC3 ) return "CV_16UC3";
    
    if( type == CV_32SC1 ) return "CV_32SC1";
    if( type == CV_32SC2 ) return "CV_32SC2"; 
    if( type == CV_32SC3 ) return "CV_32SC3";
    
    if( type == CV_32FC1 ) return "CV_32FC1";
    if( type == CV_32FC2 ) return "CV_32FC2"; 
    if( type == CV_32FC3 ) return "CV_32FC3";
    
    return "UNKNOWN";
}

/**
 * Print the OpenCV Depth 
*/
std::string opencvDepth2string( const int& depth ){
    if( depth == CV_8U )
        return "CV_8U";
    if( depth == CV_16U )
        return "CV_16U";
    if( depth == CV_32S )
        return "CV_32S";
    if( depth == CV_32F )
        return "CV_32F";
    if( depth == CV_64F )
        return "CV_64F";
    
    return "UNKNOWN";
}

/**
 * Set the image pixel
*/
void cvSetPixel( Mat& image, Point const& pix, double const& val ){
    
    if(      image.type() == CV_8UC1  ) image.at<uchar>( pix) = (uchar)val;
    else if( image.type() == CV_8UC3  ) image.at<Vec3b>( pix) = Vec3b(val,val,val);
    
    else if( image.type() == CV_16UC1 ) image.at<ushort>(pix) = (short)val;
    else if( image.type() == CV_16UC3 ) image.at<Vec<unsigned short,3> >(pix) = Vec<unsigned short,3>(val,val,val);
    
    else if( image.type() == CV_16SC1 ) image.at<short>(pix) = (short)val;
    else if( image.type() == CV_16SC3 ) image.at<Vec3s>(pix) = Vec3s(val,val,val);

    else if( image.type() == CV_32SC1  ) image.at<int>(   pix) = val;
    else if( image.type() == CV_32SC3  ) image.at<Vec3i>( pix) = Vec3i(val,val,val);
    
    else if( image.type() == CV_32FC1  ) image.at<float>( pix) = val;
    else if( image.type() == CV_32FC3  ) image.at<Vec3f>( pix) = Vec3f(val,val,val);

    else if( image.type() == CV_64FC1  ) image.at<double>( pix) = val;
    else if( image.type() == CV_64FC3  ) image.at<Vec3d>( pix) = Vec3d(val,val,val);
    
    else
        throw string("ERROR: Unknown pixel type");

}

/**
 * Get the image pixel
*/
double cvGetPixel( cv::Mat const& image, cv::Point const&  pix, const int& channel ){

    if( image.type() == CV_8UC1 )
        return image.at<uchar>(pix);
    
    if( image.type() == CV_8UC3 )
        return image.at<Vec3b>(pix)[channel];
    
    if( image.type() == CV_16UC1 )
        return image.at<unsigned short>(pix);
    
    else
        throw string("ERROR: Unknown type");

}


} // End of GEO Namespace 
