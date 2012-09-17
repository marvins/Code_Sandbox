#ifndef __SRC_EXTENSIONS_GS2_HEADER_HPP__
#define __SRC_EXTENSIONS_GS2_HEADER_HPP__

#include <string>

#ifdef DEPLOYED
#include "GeoImage.h"
#else
#include <GeoImage.h>
#endif 

namespace GEO{
namespace GS2{

std::string getAcftTailNumber( GEO::GeoImage img );

double getFocalLength( GEO::GeoImage img );


} // end of GS2 Namespace 
} // end of GEO Namespace

#endif
