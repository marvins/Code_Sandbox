#include "GS2_Header.hpp"

#include "TACID.hpp"
#include "../utilities/String_Utilities.hpp"

#include <iostream>

using namespace std;

namespace GEO{
namespace GS2{

/*
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
}*/

/**
 * In an effort to encourage development of useable helper functions, I will use as
 * many helper functions to extract this data as possible.
*
std::vector<std::pair<std::string,std::string> > query_GS2_metadata( GeoImage const& gimg ){
    
    // output information
    vector<pair<string,string> > output;
    

    **
     * The NITF 2.1 header spec is modified by ITT's ICD
     * The spec is modified so that there is a separate ICD for camera 
     * images, mosaics, and raws.
    *

    //check for a separate camera
    string camval = TACID( gimg.get_filename() ).get_product_no( );
    
    string tre = gimg.get_tre();

    //grab the acftb
    vector<pair<string,string> >  acftb_data = parse_ACFTB( tre );
    for( size_t i=0; i<acftb_data.size(); i++ )
        output.push_back( acftb_data[i] );
    acftb_data.clear();

    //grab the aimitb
    vector<pair<string,string> >  aimidb_data = parse_AIMIDB( tre );
    for( size_t i=0; i<aimidb_data.size(); i++ )
        output.push_back( aimidb_data[i] );
    aimidb_data.clear();
    
    //grab the bandsb
    vector<pair<string,string> >  bandsb_data = parse_BANDSB( tre );
    for( size_t i=0; i<bandsb_data.size(); i++ )
        output.push_back( bandsb_data[i] );
    bandsb_data.clear();
    
    
    
    return output;
}
*/

/**
 * In an effort to encourage development of useable helper functions, I will use as
 * many helper functions to extract this data as possible.
*/
std::vector<std::pair<std::string,std::string> > query_GS2_metadata( const string& fname, GDALLoader const& gimg ){
    
    // output information
    vector<pair<string,string> > output;
    

    /**
     * The NITF 2.1 header spec is modified by ITT's ICD
     * The spec is modified so that there is a separate ICD for camera 
     * images, mosaics, and raws.
    */

    //check for a separate camera
    string camval = TACID( fname ).get_product_no( );
    
    string tre = gimg.get_header_tre();

    //grab the acftb
    vector<pair<string,string> >  acftb_data = parse_ACFTB( tre );
    for( size_t i=0; i<acftb_data.size(); i++ )
        output.push_back( acftb_data[i] );
    acftb_data.clear();

    //grab the aimitb
    vector<pair<string,string> >  aimidb_data = parse_AIMIDB( tre );
    for( size_t i=0; i<aimidb_data.size(); i++ )
        output.push_back( aimidb_data[i] );
    aimidb_data.clear();
    
    //grab the bandsb
    vector<pair<string,string> >  bandsb_data = parse_BANDSB( tre );
    for( size_t i=0; i<bandsb_data.size(); i++ )
        output.push_back( bandsb_data[i] );
    bandsb_data.clear();
    
    
    
    return output;
}


std::vector<std::pair<std::string,std::string> > parse_ACFTB( const std::string& tre ){
    
    //create an output container
    vector<pair<string,string> > output;
    int len;

    //bring the pointer to the ACFTB tag line
    int idx = tre.find( "ACFTB=" );
    if( idx != string::npos ){

        //start processing arguments
        string acftb_tre = tre.substr(idx);

        //add CETAG
        len = 5;
        output.push_back( pair<string,string>( "ACFTB:CETAG", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len+1);

        //add CEL
        len = 20;
        output.push_back( pair<string,string>( "ACFTB:AC_MSN_ID", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // add AC_MSN_ID  (Aircraft Mission Identification )
        len = 10;
        output.push_back( pair<string,string>( "ACFTB:AC_TAIL_NO", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add AC_TO (Aircraft Takeoff)
        len = 12;
        output.push_back( pair<string,string>( "ACFTB:AC_TO", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add SENSOR_ID_TYPE
        len = 4;
        output.push_back( pair<string,string>( "ACFTB:SENSOR_ID_TYPE", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // add SENSOR_ID
        len = 6;
        output.push_back( pair<string,string>( "ACFTB:SENSOR_ID", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // SCENE_SOURCE
        len = 1;
        output.push_back( pair<string,string>( "ACFTB:SCENE_SOURCE", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add SCNUM (Scene Number)
        len = 6;
        output.push_back( pair<string,string>( "ACFTB:SCNUM", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // add PDATE (Processing Date)
        len = 8;
        output.push_back( pair<string,string>( "ACFTB:PDATE", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Immediate Scene Host
        len = 6;
        output.push_back( pair<string,string>( "ACFTB:IMHOSTNO", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Immediate Scene Request ID
        len = 5;
        output.push_back( pair<string,string>( "ACFTB:IMREQID", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Mission Plan Mode
        len = 3;
        output.push_back( pair<string,string>( "ACFTB:MPLAN", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Entry Location
        len = 25;
        output.push_back( pair<string,string>( "ACFTB:ENTLOC", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Location Accuracy
        len = 6;
        output.push_back( pair<string,string>( "ACFTB:LOC_ACCY", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // add Entry Elevation
        len = 6;
        output.push_back( pair<string,string>( "ACFTB:ENTELV", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Entry Elevation Units
        len = 1;
        output.push_back( pair<string,string>( "ACFTB:ELV_UNIT", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Immediate Scene Host
        len = 25;
        output.push_back( pair<string,string>( "ACFTB:EXITLOC", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Exit Elevation
        len = 6;
        output.push_back( pair<string,string>( "ACFTB:EXITELV", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // add True Map Angle
        len = 7;
        output.push_back( pair<string,string>( "ACFTB:TMAP", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Row Spacing
        len = 7;
        output.push_back( pair<string,string>( "ACFTB:ROW_SPACING", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Roc spacing units
        len = 1;
        output.push_back( pair<string,string>( "ACFTB:ROW_SPACING_UNITS", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Column Spacing
        len = 7;
        output.push_back( pair<string,string>( "ACFTB:COL_SPACING", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Column Spacing Units
        len = 1;
        output.push_back( pair<string,string>( "ACFTB:COL_SPACING_UNITS", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Focal Length
        len = 6;
        output.push_back( pair<string,string>( "ACFTB:FOCAL_LENGTH", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add True Sensor Vendor Serial Number
        len = 6;
        output.push_back( pair<string,string>( "ACFTB:SENSERIAL", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);
        
        // add Airborne Software Version
        len = 7;
        output.push_back( pair<string,string>( "ACFTB:ABSWVER", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // add Calibration Date
        len = 8;
        output.push_back( pair<string,string>( "ACFTB:CAL_DATE", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // add Patch Total
        len = 4;
        output.push_back( pair<string,string>( "ACFTB:PATCH_TOT", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

        // add MTI Total
        len = 3;
        output.push_back( pair<string,string>( "ACFTB:MTI_TOT", STR::string_trim(acftb_tre.substr(0,len))));
        acftb_tre = acftb_tre.substr(len);

    }
    
    return output;
}

std::vector<std::pair<std::string,std::string> > parse_AIMIDB( const std::string& tre ){
    
    //create an output container
    vector<pair<string,string> > output;
    int len;

    //bring the pointer to the ACFTB tag line
    int idx = tre.find( "AIMIDB=" );
    if( idx != string::npos ){

        //start processing arguments
        string aimidb_tre = tre.substr(idx);

        //add CETAG
        len = 6;
        output.push_back( pair<string,string>( "AIMIDB:CETAG", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len+1);
        
        //add ACQUISTION DATE
        len = 14;
        output.push_back( pair<string,string>( "AIMIDB:ACQUISITION_DATE", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);

        //add Mission Number
        len = 4;
        output.push_back( pair<string,string>( "AIMIDB:MISSION_NO", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);

        //add Mission Identification
        len = 10;
        output.push_back( pair<string,string>( "AIMIDB:MISSION_IDENTIFICATION", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Flight Number
        len = 2;
        output.push_back( pair<string,string>( "AIMIDB:FLIGHT_NO", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Image Operation Number
        len = 3;
        output.push_back( pair<string,string>( "AIMIDB:OP_NUM", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);

        //add Current Segment ID
        len = 2;
        output.push_back( pair<string,string>( "AIMIDB:CURRENT_SEGMENT", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Reprocess Number
        len = 2;
        output.push_back( pair<string,string>( "AIMIDB:REPRO_NUM", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add REPLAY
        len = 3;
        output.push_back( pair<string,string>( "AIMIDB:REPLAY", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Reserved 001 key
        len = 1;
        output.push_back( pair<string,string>( "AIMIDB:RESERVED_001", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Start Tile Column Number
        len = 3;
        output.push_back( pair<string,string>( "AIMIDB:START_TILE_COLUMN", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Stare Tile Row Number
        len = 5;
        output.push_back( pair<string,string>( "AIMIDB:START_TILE_ROW", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Ending Segment
        len = 2;
        output.push_back( pair<string,string>( "AIMIDB:END_SEGMENT", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Ending Tile Column Number
        len = 3;
        output.push_back( pair<string,string>( "AIMIDB:END_TILE_COLUMN", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Ending Tile Row Number
        len = 5;
        output.push_back( pair<string,string>( "AIMIDB:END_TILE_ROW", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Country
        len = 2;
        output.push_back( pair<string,string>( "AIMIDB:COUNTRY", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Reserved 002
        len = 4;
        output.push_back( pair<string,string>( "AIMIDB:RESERVED_002", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Location
        len = 11;
        output.push_back( pair<string,string>( "AIMIDB:LOCATION", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);
        
        //add Reserved 003
        len = 13;
        output.push_back( pair<string,string>( "AIMIDB:RESERVED_003", STR::string_trim(aimidb_tre.substr(0,len))));
        aimidb_tre = aimidb_tre.substr(len);


    }
    return output;
}

union str2float{

    float f;
    char s[4];
    unsigned int u;
} s2f;
 

void swap( ){

    char tmp;
    tmp = s2f.s[0];
    s2f.s[0] = s2f.s[3];
    s2f.s[3] = tmp;
    
    tmp = s2f.s[1];
    s2f.s[1] = s2f.s[2];
    s2f.s[2] = tmp;

}

       
string convert_4char2float2string( string const& str ){
    
    // add data to union
    for( size_t i=0; i<4; i++ )
        s2f.s[i] = str[i];

    // swap bits
    swap();

    return STR::num2str<float>(s2f.f);
}

std::vector<std::pair<std::string,std::string> > parse_BANDSB( const std::string& tre ){
    
    //create an output container
    vector<pair<string,string> > output;
    int len;

    //bring the pointer to the ACFTB tag line
    int idx = tre.find( "BANDSB=" );
    if( idx != string::npos ){

        //start processing arguments
        string bandsb_tre = tre.substr(idx);

        //add CETAG
        len = 6;
        output.push_back( pair<string,string>( "BANDSB:CETAG", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len+1);
        
        //add COUNT
        len = 5;
        output.push_back( pair<string,string>( "BANDSB:COUNT", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Radiometric Quantity/Data Representation
        len = 24;
        output.push_back( pair<string,string>( "BANDSB:RADIOMETRIC_QUANTITY", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
    
        
        //add Radiometric Quantity/Data Representation unit
        len = 1;
        output.push_back( pair<string,string>( "BANDSB:RADIOMETRIC_QUANTITY_UNIT", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Scale Factor
        len = 4;
        string ssout = convert_4char2float2string( bandsb_tre.substr(0,len));
        output.push_back( pair<string,string>( "BANDSB:SCALE_FACTOR", STR::string_trim(ssout)));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Scale Factor
        len = 4;
        ssout = convert_4char2float2string( bandsb_tre.substr(0,len));
        output.push_back( pair<string,string>( "BANDSB:ADDITIVE_FACTOR", STR::string_trim(ssout)));
        bandsb_tre = bandsb_tre.substr(len);
        
        bandsb_tre = bandsb_tre.substr(6);
         
        //add Row Ground Sample Distance
        len = 7;
        output.push_back( pair<string,string>( "BANDSB:ROW_GSD", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Row Ground Sample Distance Units
        len = 1;
        output.push_back( pair<string,string>( "BANDSB:ROW_GSD_UNIT", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Column Ground Sample Distance
        len = 7;
        output.push_back( pair<string,string>( "BANDSB:COL_GSD", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Col Ground Sample Distance Units
        len = 1;
        output.push_back( pair<string,string>( "BANDSB:COL_GSD_UNIT", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Spatial Response Function Across Rows
        len = 7;
        output.push_back( pair<string,string>( "BANDSB:SPT_RESP_ROW", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Spatial Response Function Across Rows Units
        len = 1;
        output.push_back( pair<string,string>( "BANDSB:SPT_RESP_UNIT_ROW", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Spatial Response Function Across Cols
        len = 7;
        output.push_back( pair<string,string>( "BANDSB:SPT_RESP_COL", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Spatial Response Function Across COL Units
        len = 1;
        output.push_back( pair<string,string>( "BANDSB:SPT_RESP_UNIT_COL", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);

        //add RESERVED FOR FUTURE USE FIELD
        len = 48;
        output.push_back( pair<string,string>( "BANDSB:DATA_FLD_1", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Existense Mask Field
        len = 4;
        output.push_back( pair<string,string>( "BANDSB:EXISTENCE_MASK", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Adjustment Surface
        len = 24;
        output.push_back( pair<string,string>( "BANDSB:RADIOMETRIC_ADJUSTMENT_SURFACE", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Adjustment Surface
        len = 4;
        output.push_back( pair<string,string>( "BANDSB:ATMOSPHERIC_ADJUSTMENT_ALTITUDE", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Lens Diameter
        len = 7;
        output.push_back( pair<string,string>( "BANDSB:DIAMETER", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Reserved for future use
        len = 32;
        output.push_back( pair<string,string>( "BANDSB:DATA_FLD_2", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        
        //add Wavelength units
        len = 1;
        output.push_back( pair<string,string>( "BANDSB:WAVE_LENGTH_UNIT", STR::string_trim(bandsb_tre.substr(0,len))));
        bandsb_tre = bandsb_tre.substr(len);
        

    }

    return output;
}


} //end of GS2 Namespace 
} //end of GEO Namespace 

