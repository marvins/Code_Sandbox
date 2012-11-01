#ifndef __SRC_EXTENSIONS_GS2_HEADER_HPP__
#define __SRC_EXTENSIONS_GS2_HEADER_HPP__

#include <string>
#include <vector>

//#include <opencv2/core/core.hpp>

#include "../io/GDALLoader.hpp"


namespace GEO{
namespace GS2{

template <typename TP1, typename TP2, typename TP3>
class triple{

    public:
        
        triple(){}
        triple( TP1 const& a, TP2 const& b, TP3 const& c ):fst(a),snd(b),trd(c){}

        TP1 fst;
        TP2 snd;
        TP3 trd;
};

typedef triple<std::string,std::string,std::string>  tre_tuple;


/**
 * Query a specific TRE string and extract all GS information
*/
std::vector<tre_tuple> parse_TRE_string( GDALLoader const& gimg, std::string const& TRE );

/**
 * Query the GeoImage for any GS2-related header information
*/
std::vector<tre_tuple> parse_GS_header( const std::string& fname, GDALLoader const& gimg );


} // end of GS2 Namespace 
} // end of GEO Namespace

#endif
