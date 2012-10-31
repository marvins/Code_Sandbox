#include "Datum.hpp"

using namespace std;

namespace GEO{

std::string datum2string( const int& datum ){

    if( datum == NAD83 ) return "NAD83";
    if( datum == WGS72 ) return "WGS72";
    if( datum == WGS84 ) return "WGS84";

    return "UNKNOWN";
}

}

