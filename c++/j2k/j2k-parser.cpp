
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
    int64_t max_len = -1;
    if( argc > 2 ){
        max_len = stoi(argv[2]);
    }

    // Open the file and extract the buffer
    uint8_t*  input_data; 
    int64_t   input_size;
    int64_t   offset = 0;
    Load_J2K_Data( input_path,
                   input_data,
                   input_size );


    if( max_len < 0 ){
        max_len = input_size;
    }
    
    // Start parsing the file
    J2K_Marker_Container container;
    container.Process_Markers( input_data,
                               offset,
                               max_len );
    

    // Exit
    return 0;
}


