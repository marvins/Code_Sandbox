#include "main.hpp"


int main( int argc, char* argv[] ){

    // process command-line arguments
    options.init( argc, argv );

    try{
    
        // start ui
        start_ui( );
        
        // start the 
        user_interface( );

    
    }catch( string e ){
        cout << e << endl;
    }
    
    // stop ui
    stop_ui( );

    return 0;
}

