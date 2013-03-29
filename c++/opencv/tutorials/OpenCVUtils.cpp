#include "OpenCVUtils.h"

int cvDepthChannel2Type( const int Depth, const int Channels ){

    if( Depth == CV_8U  && Channels == 3 )  return CV_8UC3;
    if( Depth == CV_8U  && Channels == 2 )  return CV_8UC2;
    if( Depth == CV_8U  && Channels == 1 )  return CV_8UC1;
    if( Depth == CV_16U && Channels == 3 ) return CV_16UC3;
    if( Depth == CV_16U && Channels == 2 ) return CV_16UC2;
    if( Depth == CV_16U && Channels == 1 ) return CV_16UC1;

    throw string("Error: combo not supported");

    return 0;
}
