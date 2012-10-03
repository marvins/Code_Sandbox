
///STL Libraries
#include <iostream>
#include <string>

///Personal Libraries
#include "Options.hpp"

///useful namespaces
using namespace std;


/** 
 * Main Driver
 *
 * Implements the primary status interface
*/
int main( int argc, char * argv[] ){


    ///load the configuration here
    Options options( argc, argv );
    
    ///print the configuration
    options.print();

    return 0;
}
