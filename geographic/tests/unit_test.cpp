/**
   Unit test showing how to use and implement the Geographic Utilities
 */

#include <iostream>
#include <string>

#include <Assert.h>
#include <Logger.h>
#include <string_utilities.h>

#include "structures/TEST_structures.h"
#include "utilities/TEST_utilities.h"

using namespace std;

/** Main Driver 
 *
 * @brief Entrance to the Unit Test
 */
int main(int argc, char* argv[]) {

    try {
        print_module_header("GeoImage Unit Test Framework");
        
        //begin structure module test
        TEST_structures_module();

        //begin utilities module test
        TEST_utilities_module();

        print_module_footer("GeoImage Unit Test Framework");
    
    
    }    catch (string e) {
        cout << e << endl;
    }
    
    //end of unit tests
    return 0;
}

