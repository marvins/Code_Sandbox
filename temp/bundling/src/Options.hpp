#ifndef __SRC_OPTIONS_HPP__
#define __SRC_OPTIONS_HPP__

///STL Libraries
#include <iostream>
#include <string>

///Personal Libraries
#include "FilePtr.hpp"
#include "Parser.hpp"

///useful includes
using namespace std;


/** 
 * Useful enumerations
*/
const int CAMERA_EO = 0;
const int CAMERA_IR = 1;

const int COLLECT_RAW  = 0;
const int COLLECT_NITF = 1;

const int PROGRAM_EVAL   = 0;
const int PROGRAM_BUNDLE = 1;

/**
 * @class Options
 *
 * The required configuration parameters for the program.
*/
class Options{

    public:
        
        /**
         * Parameterized Constructor
        */
        Options( const int argc, char** argv );
        

        /** 
         * Print the current configuration
         * NOTE: For debugging only
        */
        void print()const;
        
        /**
         * Print the help menu
        */
        void usage();
        
        /**************************************************************/
        /*                        CORE PARAMETERS                     */
        /**************************************************************/

        /**
         * Program Modes
         * 
         * Available options:
         * - PROGRAM_EVAL
         * - PROGRAM_BUNDLE
         */
        int program_mode;
        
        
        int collect_type;              /*< Collection type */
        string collect_camera_path;    /*< Collection camera path */
        string collect_name;           /*< Name of the collect */
        
        string status_filename; /*< Name of the file which manages the program state */
        string config_filename; /*< Name of the configuration file */
        
        string camera_type;     /*< Type of camera we are using */
        
        PSR::Parser parser;          /*<  Configuration file parser  */

        string input_base;      /*< Base directory to search for cameras */

        int number_eo_frames;   /*< Number of eo frames per step  */
        int number_ir_frames;   /*< Number of ir frames per step  */

        int image_depth;        /*< Image depth */
        
        string sensor_serial;
        bool   sensor_serial_found;

        /**************************************************************/
        /*                      EVAL-ONLY PARAMETERS                  */
        /**************************************************************/
        
        bool   eval_use_context_file; /*< Whether or not to use a context file */
        string eval_context_filename; /*< Name of the context file */
        bool   eval_refresh_context;  /*< Whether or not to refresh the context file. */
        
        /**************************************************************/
        /*                     BUNDLE-ONLY PARAMETERS                 */
        /**************************************************************/

        int number_bundles;     /*< Number of bundles to collect */
        int max_bundle_limit;   /*< The max number of bundles to search before stopping the search  */

        string output_filename;
    
    
    private:

        void load_eval_configuration();
        void load_bundle_configuration();

};


#endif
