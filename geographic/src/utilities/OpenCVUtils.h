#ifndef __SRC_OPENCV_ENUMS_H__
#define __SRC_OPENCV_ENUMS_H__

#include <string>

int cvDepthChannel2Type( const int Depth, const int Channels );

std::string opencvType2string( const int& type );

std::string opencvDepth2string( const int& depth );


#endif
