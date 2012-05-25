#include "OpenCVUtils.h"


#include <cv.h>
#include <cvaux.h>
#include <highgui.h>

using namespace cv;

#include <iostream>
using namespace std;

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
