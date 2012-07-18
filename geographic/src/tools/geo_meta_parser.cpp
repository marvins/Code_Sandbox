#include <iostream>
#include <string>
#include <vector>

#include "geo_meta_parser/ui.h"
#include "geo_meta_parser/utils.h"

using namespace std;

int main( int argc, char * argv[] ){
    
    /**
     * Convert args to vector list
    */
    vector<string> args;
    for( int i=0; i<argc; i++ )
        args.push_back(argv[i]);

    /**
     * Process Arguments
    */
    Options configuration;
    process_arguments( args, configuration );


    /**
     * Fire up NCurses
    */
    init_gui( configuration );

    /**
     * Start the main menu
    */
    main_menu( args, configuration );
    

    /**
     * Clean up NCurses
    */
    stop_gui( configuration );
    
    return 0;
}
