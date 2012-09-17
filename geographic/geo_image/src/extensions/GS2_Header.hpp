#ifndef __SRC_EXTENSIONS_GS2_HEADER_HPP__
#define __SRC_EXTENSIONS_GS2_HEADER_HPP__

#include <string>

#ifdef DEPLOYED
#include "GeoImage.h"
#else
#include <GeoImage.h>
#endif 

#include <opencv2/core/core.hpp>

namespace GEO{
namespace GS2{

std::string getAcftTailNumber( GEO::GeoImage const& img );

double getFocalLength( GEO::GeoImage const& img );

cv::Mat getSensorPosition( GEO::GeoImage const& img );

} // end of GS2 Namespace 
} // end of GEO Namespace

#endif
