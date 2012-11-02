#include "GS2_Header.hpp"

#include "TACID.hpp"
#include "GS_ICD_Reference.cpp"

#include "../utilities/String_Utilities.hpp"

#include <iostream>

using namespace std;

namespace GEO{
namespace GS2{


union TRE_Converter{
    
    char   str[4];
    float  f;
    int    i;
    double d;

} tre_converter;


string pop_TRE_entry( string& tre_string, const int& size, const string& type ){
      
    string output;
    
    if( (int)tre_string.size() < size ){
        return "__NOT_ENOUGH_SPACE__";
    }

    if( type == "BCS-A" || type == "BCS-N" ){
        output = STR::string_trim(tre_string.substr(0,size));
    }
    else if( type == "IEEE32F" ){
        output = tre_string.substr(0,size);
        for( int i=0; i<4; i++ )
            tre_converter.str[3-i] = output[i];
        output = STR::num2str( tre_converter.f );
    }   
    else if( type == "DUMP" ){
        output = tre_string;
    }
    else{    
        output = STR::string_trim(tre_string.substr(0,size));
    }
    
    if( type != "DUMP" )
        tre_string = tre_string.substr(size);
    else
        tre_string.clear();

    return output;
}

vector<tre_tuple> parse_TRE_string( GDALLoader const& gimg, string const& TRE ){
    
    // grab the tre string
    string tre_string = gimg.get_header_tre( TRE );

    // create out output object
    vector<tre_tuple> output;
    output.clear();

    if( tre_string.size() <= 0 )
        return output;
    
    // iterate through every TRE Tag
    if( TRE == "ACFTB" ){
        for( int i=0; i<ACFTB_ICD_SIZE; i++ )
            output.push_back( tre_tuple( TRE + string(":") + ACFTB_ICD_Reference[i][0], 
                                         pop_TRE_entry(tre_string, STR::str2num<int>(ACFTB_ICD_Reference[i][2]), ACFTB_ICD_Reference[i][3]),
                                         ACFTB_ICD_Reference[i][1]));
    }
    else if( TRE == "AIMIDB" ){    
        for( int i=0; i<AIMIDB_ICD_SIZE; i++ )
            output.push_back( tre_tuple( TRE + string(":") + AIMIDB_ICD_Reference[i][0], 
                                         pop_TRE_entry(tre_string, STR::str2num<int>(AIMIDB_ICD_Reference[i][2]), AIMIDB_ICD_Reference[i][3]),
                                         AIMIDB_ICD_Reference[i][1]));
    }
    else if( TRE == "BANDSB" ){
        for( int i=0; i<BANDSB_ICD_SIZE; i++ )
            output.push_back( tre_tuple( TRE + string(":") + BANDSB_ICD_Reference[i][0], 
                                         pop_TRE_entry(tre_string, STR::str2num<int>(BANDSB_ICD_Reference[i][2]), BANDSB_ICD_Reference[i][3]),
                                         BANDSB_ICD_Reference[i][1]));
    }
    else if( TRE == "BLOCKA" ){
        for( int i=0; i<BLOCKA_ICD_SIZE; i++ )
            output.push_back( tre_tuple( TRE + string(":") + BLOCKA_ICD_Reference[i][0], 
                                         pop_TRE_entry(tre_string, STR::str2num<int>(BLOCKA_ICD_Reference[i][2]), BLOCKA_ICD_Reference[i][3]),
                                         BLOCKA_ICD_Reference[i][1]));
    }
    
    else if( TRE == "BNDPLB" ){
        for( int i=0; i<BNDPLB_ICD_SIZE; i++ )
            output.push_back( tre_tuple( TRE + string(":") + BNDPLB_ICD_Reference[i][0], 
                                         pop_TRE_entry(tre_string, STR::str2num<int>(BNDPLB_ICD_Reference[i][2]), BNDPLB_ICD_Reference[i][3]),
                                         BNDPLB_ICD_Reference[i][1]));
    }
    else if( TRE == "ENGRDA" ){
        for( int i=0; i<ENGRDA_ICD_SIZE; i++ )
            output.push_back( tre_tuple( TRE + string(":") + ENGRDA_ICD_Reference[i][0], 
                                         pop_TRE_entry(tre_string, STR::str2num<int>(ENGRDA_ICD_Reference[i][2]), ENGRDA_ICD_Reference[i][3]),
                                         ENGRDA_ICD_Reference[i][1]));
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
    
    //grab the aimidb
    tre_data = parse_TRE_string( gimg, "AIMIDB" );
    for( size_t i=0; i<tre_data.size(); i++ )
        output.push_back( tre_data[i] );
    tre_data.clear();
    
    //grab the bandsb
    tre_data = parse_TRE_string( gimg, "BANDSB" );
    for( size_t i=0; i<tre_data.size(); i++ )
        output.push_back( tre_data[i] );
    tre_data.clear();
    
    //grab the bndplb
    tre_data = parse_TRE_string( gimg, "BNDPLB" );
    for( size_t i=0; i<tre_data.size(); i++ )
        output.push_back( tre_data[i] );
    tre_data.clear();
    
    //grab the bandsb
    tre_data = parse_TRE_string( gimg, "ENGRDA" );
    for( size_t i=0; i<tre_data.size(); i++ )
        output.push_back( tre_data[i] );
    tre_data.clear();

    return output;
}


} //end of GS2 Namespace 
} //end of GEO Namespace 

