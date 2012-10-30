#ifndef __SRC_COORDINATE_DATUM_HPP__
#define __SRC_COORDINATE_DATUM_HPP__

#include <string>

namespace GEO{
    
    enum DATUM_TYPES{
        NAD83 = 0,
        WGS72 = 1,
        WGS84 = 2
    };


std::string datum2string( const int& datum );

}// End of GEO Namespace


#endif

