/**
 * Main program for the Geo Metadata Parser utility 
 *
 * @author Marvin Smith
 * @date   23 October 2012
*/

//STL Libraries
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Personal Libraries
#include "geo_meta_parser/io.hpp"
#include "geo_meta_parser/utilities.hpp"

using namespace std;


/**
 * Main Driver
*/
int main( int argc, char * argv[] ){

    /**
     * Convert args to vector list
     */
    vector<string> args = args2vector( argc, argv );


    /**
     * Process Arguments
    */
    Options options;
    
    try{
        
        // process the command-line arguments
        process_arguments( args, options );
        
        // parse command-line arguments
        parse_metadata( options );
        

    } catch( string e ){
        cout << e << endl;
    }

    
    return 0;
}
