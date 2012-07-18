#include <iostream>

#include "geo_meta_parser/ui.h"

using namespace std;

int main( int argc, char * argv[] ){

    /**
     * Fire up NCurses
    */
    init_gui();

    /**
     * Start the main menu
    */
    main_menu();
    

    /**
     * Clean up NCurses
    */
    stop_gui();
    
    return 0;
}
