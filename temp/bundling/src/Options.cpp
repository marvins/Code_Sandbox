#include "Options.hpp"

#include "IO.hpp"

#include <cstdlib>

/**
 * Initializes the Parser and loads the core file options.
 *
 * Note that for the eval and bundle modes, it will call their respective
 * configuration functions.
*/
Options::Options( const int argc, char** argv ){

    bool found;

    //set defaults
    status_filename = "/opt/gorgonstare/bin/.frame_analyzer";
    config_filename = "/opt/gorgonstare/bin/frame_analyzer.cfg";

    //initialize the parser
    parser.init( argc, argv, config_filename );
    

    //query path information
    input_base = parser.getItem_string("INPUT_BASE", found);
    if( found == false )
        throw string("ERROR: no INPUT_BASE found in configuration");
    
    
    output_filename = parser.getItem_string("OUTPUT_FILENAME", found);
    if( found == false )
        throw string("ERROR: no OUTPUT_FILENAME found in configuration");

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
    
    // load the image frame depth
    image_depth = parser.getItem_int("IMAGE_DEPTH", found );
    if( found == false )
        throw string("ERROR: no IMAGE_DEPTH found in configuration");

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

    //query program mode
    string pm = parser.getItem_string("PROGRAM_MODE", found );
    if( found == false )
        throw string("ERROR: no PROGRAM_MODE found in configuration");

    if( pm == "EVAL" ){
        program_mode = PROGRAM_EVAL;
        load_eval_configuration();
    }
    else if( pm == "BUNDLE" ){
        program_mode = PROGRAM_BUNDLE;
        load_bundle_configuration();
    }
     else
         throw string("ERROR: PROGRAM_MODE has invalid option");


}

void Options::load_eval_configuration(){

    
    bool found;
    
    /**
     * The first major issue is whether or not we want to use a context
     * file to manage our data.
    */
    eval_use_context_file = parser.getItem_bool("USE_CONTEXT_FILE", found );
    if( found == false )
        throw string("ERROR: USE_CONTEXT_FILE not found in configuration");

     // if we want to use a context file, then we need to acquire the filename
     if( eval_use_context_file == true ){
        
        //query the filename
        eval_context_filename = parser.getItem_string("CONTEXT_FILENAME", found );
        if( found == false )
            throw string("ERROR: CONTEXT_FILENAME was not found.  If USE_CONTEXT_FILE=true, then you must include CONTEXT_FILENAME");
        
        //query whether or not we which to refresh the context file
        eval_refresh_context = parser.getItem_bool("NEW_CONTEXT", found);
        if( found == false )
            throw string("ERROR: NEW_CONTEXT was not found. If USE_CONTEXT=true, then you must include CONTEXT_FILENAME");
        
        //if we do wish to refresh the context file, then we should delete the file now so that during the 
        //context file load, it doesn't find it, prompting the file to be generated.
        if( eval_refresh_context == true )
            file_delete( eval_context_filename );

     }



}

void Options::load_bundle_configuration(){

    bool found;

    //load the number of bundles to collect
    number_bundles = parser.getItem_int("NUMBER_BUNDLES", found);
    if( found == false )
        throw string("ERROR: no NUMBER_BUNDLES found in configuration");

    
    //load the max number of bundles to search for
    max_bundle_limit = parser.getItem_int("MAX_BUNDLE_LIMIT", found );
    if( found == false )
        throw string("ERROR: no MAX_BUNDLE_LIMIT found in configuration" );
   

}


/** 
 * Print the current configuration
 * NOTE: For debugging only
 */
void Options::print()const{

    cout << "Current Configuration" << endl;
    cout << endl;
    cout << "Core Options" << endl;
    cout << "------------" << endl;
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
    cout << endl;
    cout << "  -COLLECT_TYPE=" << collect_type << endl;
    cout << "  -COLLECT_CAMERA_PATH=" << collect_camera_path << endl;
    cout << "  -IMAGE_DEPTH=" << image_depth << endl; 
    cout << endl;
    if( program_mode == PROGRAM_EVAL ){
        cout << "Evaluation Options" << endl;
        cout << "------------------" << endl;
        cout << endl;
        cout << " -EVAL_USE_CONTEXT_FILE=";
        if( eval_use_context_file == true ) cout << "TRUE"  << endl;
        else                                cout << "FALSE" << endl;
        
        cout << " -EVAL_CONTEXT_FILENAME=" << eval_context_filename << endl;
        
        cout << " -NEW_CONTEXT=";
        if( eval_refresh_context == true ) cout << "TRUE"  << endl;
        else                               cout << "FALSE" << endl;

        cout << endl;
    }
    else if( program_mode == PROGRAM_BUNDLE ){
        cout << "Bundle Options" << endl;
        cout << "--------------" << endl;
        cout << endl;
        cout << " - NUMBER_BUNDLES=" << number_bundles << endl;
        cout << " - MAX_BUNDLE_LIMIT=" << max_bundle_limit << endl;
        
        cout << endl;
    }
    

}


/**
 * Print the help menu
 */
void Options::usage(){

    cout << "Usage" << endl;
    cout << endl;
}



