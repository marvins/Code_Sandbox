#include "GS2_Header.hpp"

#include "TACID.hpp"
#include "GS_ICD_Reference.hpp"

#include "../utilities/String_Utilities.hpp"

#include <iostream>

using namespace std;

namespace GEO{
namespace GS2{

string pop_TRE_entry( string& tre_string, const int& size, const string& type ){
       
    string output;

    if( type == "BCS-A" || type == "BCS-N" ){
        output = STR::string_trim(tre_string.substr(0,size));
        tre_string = tre_string.substr(size);
    }

    return output;
}


vector<tre_tuple> parse_TRE_string( GDALLoader const& gimg, string const& TRE ){

    // grab the tre string
    string tre_string = gimg.get_header_tre( TRE );
    
    // create out output object
    vector<tre_tuple> output;
    
    // iterate through every TRE Tag
    if( TRE == "ACFTB" ){
        
        for( int i=0; i<ACFTB_ICD_SIZE; i++ )
            output.push_back( tre_tuple( TRE + string(":") + ACFTB_ICD_Reference[i][0], 
                                         pop_TRE_entry(tre_string, STR::str2num<int>(ACFTB_ICD_Reference[i][2]), ACFTB_ICD_Reference[i][3]),
                                         ACFTB_ICD_Reference[i][1]));
    }

    return output;
}

/**
 * In an effort to encourage development of useable helper functions, I will use as
 * many helper functions to extract this data as possible.
*/
std::vector<tre_tuple> parse_GS_header( const string& fname, GDALLoader const& gimg ){
    
    // output information
    vector<tre_tuple> output;
    

    /**
     * The NITF 2.1 header spec is modified by ITT's ICD
     * The spec is modified so that there is a separate ICD for camera 
     * images, mosaics, and raws.
    */

    //check for a separate camera
    string camval = TACID( fname ).get_product_no( );
    
    vector<tre_tuple> tre_data;

    //grab the acftb
    tre_data = parse_TRE_string( gimg, "ACFTB" );
    for( size_t i=0; i<tre_data.size(); i++ )
        output.push_back( tre_data[i] );
    tre_data.clear();
    
    return output;
}


} //end of GS2 Namespace 
} //end of GEO Namespace 

