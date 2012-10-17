#include "Options.hpp"

#include <cstdlib>

Options::Options( const int argc, char** argv ){

    bool found;

    //set defaults
    status_filename = ".frame_analyzer";
    config_filename = "frame_analyzer.cfg";

    //initialize the parser
    parser.init( argc, argv, config_filename );

    //query path information
    input_base = parser.getItem_string("INPUT_BASE", found);
    if( found == false )
        throw string("ERROR: no INPUT_BASE found in configuration");

    collect_name = parser.getItem_string("COLLECT_NAME", found);
    if( found == false )
        throw string("ERROR: no COLLECT_NAME found in configuration");

    //load the camera type
    camera_type = parser.getItem_string("CAMERA_TYPE", found);
    if( found == false )
        throw string("ERROR: no CAMERA_TYPE found in configuration");

    //load the number of eo and ir frames
    number_eo_frames = parser.getItem_int("NUMBER_EO_FRAMES", found );
    if( found == false )
        throw string("ERROR: no NUMBER_EO_FRAMES found in configuration");

    number_ir_frames = parser.getItem_int("NUMBER_IR_FRAMES", found );
    if( found == false )
        throw string("ERROR: no NUMBER_IR_FRAMES found in configuration");
    
    //load the number of bundles to collect
    number_bundles = parser.getItem_int("NUMBER_BUNDLES", found);
    if( found == false )
        throw string("ERROR: no NUMBER_BUNDLES found in configuration");
    
    //load the max number of bundles to search for
    max_bundle_limit = parser.getItem_int("MAX_BUNDLE_LIMIT", found );
    if( found == false )
        throw string("ERROR: no MAX_BUNDLE_LIMIT found in configuration" );
    
    // load the type of collection 
    string ctype = parser.getItem_string("COLLECT_TYPE", found );
    if( found == false )
        throw string("ERROR: no COLLECT_TYPE found in configuration");

    if( ctype == "RAW" )
        collect_type = COLLECT_RAW;
    else if( ctype == "NITF" )
        collect_type = COLLECT_NITF;
    else
        throw string("ERROR: Unsupported COLLECT_TYPE value");
    
    // load the collection path
    collect_camera_path = parser.getItem_string("COLLECT_CAMERA_PATH", found);
    if( found == false )
        throw string("ERROR: no COLLECT_CAMERA_PATH found in configuration");

}


/** 
 * Print the current configuration
 * NOTE: For debugging only
 */
void Options::print()const{

    cout << "Current Configuration" << endl;
    cout << endl;
    cout << "  - status_filename = " << status_filename << endl;
    cout << "  - config_filename = " << config_filename << endl;
    cout << endl;
    cout << "  - CAMERA_TYPE=" << camera_type << endl;
    cout << "  - NUMBER_BUNDLES=" << number_bundles << endl;
    cout << endl;
    cout << "  - INPUT_BASE=" << input_base << endl;
    cout << "  - COLLECT_NAME=" << collect_name << endl;
    cout << endl;
    cout << "  - Number EO Frames=" << number_eo_frames << endl;
    cout << "  - Number IR Frames=" << number_ir_frames << endl;
    cout << endl;
    cout << "  - MAX_BUNDLE_LIMIT=" << max_bundle_limit << endl;
    cout << endl;
    cout << "  -COLLECT_TYPE=" << collect_type << endl;
    cout << "  -COLLECT_CAMERA_PATH=" << collect_camera_path << endl;

}


/**
 * Print the help menu
 */
void Options::usage(){

    cout << "Usage" << endl;
    cout << endl;
}



