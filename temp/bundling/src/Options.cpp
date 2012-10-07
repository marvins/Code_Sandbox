#include "Options.hpp"

Options::Options( const int argc, char** argv ){

    bool found;

    //set defaults
    status_filename = ".frame_analyzer";
    config_filename = "data/frame_analyzer.cfg";

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
    cout << endl;
    cout << "  - INPUT_BASE=" << input_base << endl;
    cout << "  - COLLECT_NAME=" << collect_name << endl;
    cout << endl;
    cout << "  - Number EO Frames=" << number_eo_frames << endl;
    cout << "  - Number IR Frames=" << number_ir_frames << endl;
}


/**
 * Print the help menu
 */
void Options::usage(){

    cout << "Usage" << endl;
    cout << endl;
}



