/**
   Unit test showing how to use and implement the Geographic Utilities
 */

#include <iostream>
#include <string>

#include "../unit_test/src/Assert.h"
#include "../unit_test/src/Logger.h"
#include "../useful_stuff/string_functions/string_utilities.h"

#include "TEST_structures.h"
#include "TEST_utilities.h"

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

