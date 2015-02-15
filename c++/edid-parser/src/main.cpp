/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    2/10/2015
*/

// C++ Standard Libraries
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

// Libraries
#include "EDID_Container.hpp"
#include "String_Utilities.hpp"


using namespace std;


int main( int argc, char* argv[] ){

    // Parse String
    string edid_string="00ffffffffffff000610c99c000000000c140103801d13780af595a3554f9c260f505400000001010101010101010101010101010101521c00a050201730302036001eb210000018000000010006102000000000000000000a20000000fe004e31333349362d4c31300a2020000000fc00436f6c6f72204c43440a202020005d";

    // Convert to char array
    std::vector<uint8_t> edid_data = convert_HexStr2CharArray( edid_string );

    // Pass to the parser
    EDID_Container  container( edid_data );

    // Return Success
    return 0;

}

