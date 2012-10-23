#ifndef __SRC_EXTENSIONS_GS2_HEADER_HPP__
#define __SRC_EXTENSIONS_GS2_HEADER_HPP__

#include <string>
#include <vector>
#include <utility>

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

/**
 * Query the GeoImage for any GS2-related header information
*/
std::vector<std::pair<std::string,std::string> > query_GS2_metadata( GeoImage const& gimg );

} // end of GS2 Namespace 
} // end of GEO Namespace

#endif
