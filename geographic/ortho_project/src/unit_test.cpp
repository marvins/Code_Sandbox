/**
   Unit test showing how to use and implement the Geographic Utilities
 */

#include <iostream>
#include <string>

#include "unit_testing/Logger.hpp"

//#include "dem/TEST_dem.h"

using namespace std;

/** Main Driver 
 *
 * @brief Entrance to the Unit Test
 */
int main(int argc, char* argv[]) {

    try {
        print_module_header("GeoImage Unit Test Framework");
        
        //begin dem module test
        //TEST_dem_module();

        //begin extensions module test
        //TEST_extensions_module();

        //begin structure module test
        //TEST_structures_module();

        //begin utilities module test
        //TEST_utilities_module();

        print_module_footer("GeoImage Unit Test Framework");
    
    
    }    catch (string e) {
        cout << e << endl;
    }
    
    //end of unit tests
    return 0;
}

