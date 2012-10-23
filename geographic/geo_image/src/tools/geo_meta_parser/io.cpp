#include "io.hpp"
#include "File_Utilities.hpp"
#include "string_utilities.hpp"

#include <GeoImage.hpp>

#include <fstream>

using namespace std;


void parse_metadata( Options const& options ){

    //output metadata key=value pairs
    vector<pair<string,string> > info;
    
    
    // make sure that the file exists
    if( file_exists( options.filename ) == false )
        throw string(string("ERROR: ")+options.filename+string(" does not exist."));
    
    //open the geo-image object and pull out the standard metadata
    GEO::GeoImage gimg = GEO::GeoImage( options.filename, true );
    info = gimg.get_header()->get_header_data();
    for( size_t i=0; i<info.size(); i++ ){
        info[i].first  = string_trim(info[i].first );
        info[i].second = string_trim(info[i].second);
    }

    //print the primary header info
    cout << "Primary Image Header Metadata" << endl;
    print_header(cout, info);
    cout << endl;

    if( GEO::GS2::TACID::isValidTACID( gimg.get_filename() ) ){
        
        //extract TRE Header Data
        cout << "GS2 TRE Header Metadata" << endl;
        info = GEO::GS2::query_GS2_metadata( gimg );
        print_header( cout, info );
        cout << endl;
        
        //extract the TACID
        cout << "GS2 TACID Information" << endl;
        info = GEO::GS2::TACID( gimg.get_filename() ).query_TACID_data( );
        print_header( cout, info );
    }


}



