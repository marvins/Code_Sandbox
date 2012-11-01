#include "io.hpp"
#include "../../io/GDALLoader.hpp"
#include "../../extensions/GS2_Header.hpp"
#include "../../extensions/TACID.hpp"
#include "../../utilities/File_Utilities.hpp"
#include "../../utilities/String_Utilities.hpp"

#include <fstream>

using namespace std;
using namespace GEO::GS2;

void parse_metadata( Options const& options ){

    //output metadata key=value pairs
    vector<tre_tuple> info;
    
    
    // make sure that the file exists
    if( GEO::STR::file_exists( options.filename ) == false )
        throw string(string("ERROR: ")+options.filename+string(" does not exist."));
    
    //open the geo-image object and pull out the standard metadata
    GEO::GDALLoader gdalloader( options.filename );

    vector<pair<string,string> > nitf_data;
    nitf_data = gdalloader.get_header_data();
    for( size_t i=0; i<nitf_data.size(); i++ ){

        tre_tuple out;
        out.fst = GEO::STR::string_trim(nitf_data[i].first);
        out.snd = GEO::STR::string_trim(nitf_data[i].second);
        out.trd = "";
        info.push_back(out);
    }
    nitf_data.clear();

    //print the primary header info
    cout << "Primary Image Header Metadata" << endl;
    print_header(cout, info);
    info.clear();
    cout << endl;

    if( GEO::GS2::TACID::isValidTACID( options.filename ) ){

        //extract TRE Header Data
        cout << "GS2 TRE Header Metadata" << endl;
        info = GEO::GS2::parse_GS_header( options.filename, gdalloader );
        print_header( cout, info );
        info.clear();
        cout << endl;

        //extract the TACID
        cout << "GS2 TACID Information" << endl;
        nitf_data = GEO::GS2::TACID( options.filename ).query_TACID_data( );
        
        for( size_t i=0; i<nitf_data.size(); i++ ){
            tre_tuple out;
            out.fst = GEO::STR::string_trim(nitf_data[i].first);
            out.snd = GEO::STR::string_trim(nitf_data[i].second);
            out.trd = "";
            info.push_back(out);
        }
        print_header( cout, info );
        info.clear();
    }
    

}



