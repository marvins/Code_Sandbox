
// C++ Libraries
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <string>

// Project Libraries
#include "J2K_Parsers.hpp"
#include "Utilities.hpp"


using namespace std;


/**
 * @brief Main Application
 */
int main( int argc, char* argv[] )
{
    // Open the file
    std::string input_path = argv[1];

    // Open the file and extract the buffer
    uint8_t*  input_data; 
    int64_t   input_size;
    int64_t   offset = 0;
    Load_J2K_Data( input_path,
                   input_data,
                   input_size );


    // Start parsing the file
    J2K_Marker_Container container;
    container.Process_Markers( input_data,
                               offset,
                               input_size );
    

    // Exit
    return 0;
}


