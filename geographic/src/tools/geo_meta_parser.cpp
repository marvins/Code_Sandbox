
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <ncurses.h>

#include "geo_meta_parser/ui.h"
#include "geo_meta_parser/utils.h"

using namespace std;

int main( int argc, char * argv[] ){

    /**
     * Convert args to vector list
     */
    vector<pair<string,string> > args;
    string str;
    for( int i=0; i<argc; i++ ){
        str = argv[i];
        std::transform(str.begin(), str.end(),str.begin(), ::tolower);
        args.push_back(pair<string,string>(str, argv[i]));
    }

    /**
     * Process Arguments
     */
    Options configuration;
    
    try{

        process_arguments( args, configuration );

        if( configuration.debug_level >= 2 ){
            configuration.print();
            cin.get();
        }

        /**
         * Fire up NCurses
         */
        init_gui( configuration );

        init_pair( active_window_pair, GEO_WHITE, GEO_BLACK );
        init_pair(  backg_window_pair, GEO_BLUE , GEO_BLACK);
        
        
        /**
         * Start the main menu
         */
        main_menu( configuration );


        /**
         * Clean up NCurses
         */
        stop_gui( configuration );

    } catch( string e ){
        stop_gui( configuration );
        cout << e << endl;
    }

    
    cout << "EXITING NOW" << endl;
    
    return 0;
}
