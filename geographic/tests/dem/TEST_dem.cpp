#include "TEST_dem.h"

//#include <iostream>
#include <string> 

#include <Logger.h>

using namespace std;



/** 
 * Beginning of the structures module test
 *
 * Modules covered
 * - PixelType
*/
int TEST_dem_constructors( string& note );

void TEST_dem_module(){
   
    int result;
    string note;

    print_module_header("DEM");

    result = TEST_dem_constructors( note );
    print_test_results( "DEM    Constructors", result, note );


    print_module_footer("DEM");   

}

int TEST_dem_constructors( string& note ){
    
    
    note = "Operation failed";
    return false;
}

