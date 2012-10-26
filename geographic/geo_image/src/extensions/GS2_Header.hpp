#ifndef __SRC_EXTENSIONS_GS2_HEADER_HPP__
#define __SRC_EXTENSIONS_GS2_HEADER_HPP__

#include <string>
#include <vector>
#include <utility>

//#include <opencv2/core/core.hpp>

#include "../io/GDALLoader.hpp"


namespace GEO{
namespace GS2{

//std::string getAcftTailNumber( GEO::GeoImage const& img );

//double getFocalLength( GEO::GeoImage const& img );

//cv::Mat getSensorPosition( GEO::GeoImage const& img );

/**
 * Query the GeoImage for any GS2-related header information
*/
std::vector<std::pair<std::string,std::string> > query_GS2_metadata( const std::string& fname, GDALLoader const& gimg );

/**
 * Parse the ACFTB Header
*/
std::vector<std::pair<std::string,std::string> > parse_ACFTB( const std::string& tre );

/**
 * Parse the AIMIDB header
*/
std::vector<std::pair<std::string,std::string> > parse_AIMIDB( const std::string& tre );


/**
 * Parse the BANDSB Header
*/
std::vector<std::pair<std::string,std::string> > parse_BANDSB( const std::string& tre );


} // end of GS2 Namespace 
} // end of GEO Namespace

#endif
