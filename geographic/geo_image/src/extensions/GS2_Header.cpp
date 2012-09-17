#include "GS2_Header.hpp"
#include "../utilities/string_utils.hpp"

#include <iostream>

using namespace std;
using namespace cv;

namespace GEO{
namespace GS2{

std::string getAcftTailNumber( GEO::GeoImage const& img ){
    
    //recover the TRE Tag
    string TRE = img.get_tre();

    //look for ACFTB
    int pos = TRE.find("ACFTB");
    
    //in the icd, the index is 20 bytes from the start of the TRE
    int idx = 20;

    return GEO::STR::trim(TRE.substr( idx, 10));
}


double getFocalLength( GEO::GeoImage const& img ){
    
    //recover the TRE Tag
    string TRE = img.get_tre();

    //look for ACFTB
    int pos = TRE.find("ACFTB");

    //this is the position from the start of the TRE.  The compiler will optimize this out
    int idx = 20+10+6+12+4+6+1+6+8+6+5+3+25+6+7+25+13+8+8;

    return GEO::STR::str2num<double>(GEO::STR::trim(TRE.substr(idx,6)))/100.0;
}

cv::Mat getSensorPosition( GEO::GeoImage const& img ){

    //create output matrix element
    Mat output(4, 1, CV_64FC1);
    
    //recover the tre tag
    string TRE = img.get_tre();
    
    //look for SENSRA
    int pos = TRE.find("SENSRA");

    //this is the location from the start of the TRE. The compiler will optimize this out
    int idx = pos + 7 + 8 + 8 + 6;

    cout << "SUB: " << GEO::STR::trim(TRE.substr(idx,10)) << endl; 

    return output;
}


} //end of GS2 Namespace 
} //end of GEO Namespace 

