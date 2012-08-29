#include "GS2_Header.hpp"
#include "../utilities/string_utils.hpp"

#include <iostream>

using namespace std;

namespace GEO{
namespace GS2{

std::string getAcftTailNumber( GEO::GeoImage img ){
    
    //recover the TRE Tag
    string TRE = img.get_tre();

    //look for ACFTB
    int pos = TRE.find("ACFTB");
    int idx = 20;

    return GEO::STR::trim(TRE.substr( idx, 10));
}


double getFocalLength( GEO::GeoImage img ){
    
    //recover the TRE Tag
    string TRE = img.get_tre();

    //look for ACFTB
    int pos = TRE.find("ACFTB");
    int idx = 20+10+6+12+4+6+1+6+8+6+5+3+25+6+7+25+13+8+8;

    return GEO::STR::str2num<double>(GEO::STR::trim(TRE.substr(idx,6)))/100.0;
}


} //end of GS2 Namespace 
} //end of GEO Namespace 

