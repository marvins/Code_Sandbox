#include "io.hpp"
#include "../../io/GDALLoader.hpp"
#include "../../extensions/GS2_Header.hpp"
#include "../../extensions/TACID.hpp"
#include "../../utilities/File_Utilities.hpp"
#include "../../utilities/String_Utilities.hpp"

#include <fstream>

using namespace std;


void parse_metadata( Options const& options ){

    //output metadata key=value pairs
    vector<pair<string,string> > info;
    
    
    // make sure that the file exists
    if( GEO::STR::file_exists( options.filename ) == false )
        throw string(string("ERROR: ")+options.filename+string(" does not exist."));
    
    //open the geo-image object and pull out the standard metadata
    GEO::GDALLoader gdalloader( options.filename );

    info = gdalloader.get_header_data();
    for( size_t i=0; i<info.size(); i++ ){
        info[i].first  = GEO::STR::string_trim(info[i].first );
        info[i].second = GEO::STR::string_trim(info[i].second);
    }

    //print the primary header info
    cout << "Primary Image Header Metadata" << endl;
    print_header(cout, info);
    cout << endl;
    
    if( GEO::GS2::TACID::isValidTACID( options.filename ) ){
        
        //extract TRE Header Data
        cout << "GS2 TRE Header Metadata" << endl;
        info = GEO::GS2::query_GS2_metadata( options.filename, gdalloader );
        print_header( cout, info );
        cout << endl;
        
        //extract the TACID
        cout << "GS2 TACID Information" << endl;
        info = GEO::GS2::TACID( options.filename ).query_TACID_data( );
        print_header( cout, info );
    }

}



