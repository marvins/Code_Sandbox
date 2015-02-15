#ifndef __STRING_UTILITIES_HPP__
#define __STRING_UTILITIES_HPP__

// C++ Standard Libraries
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

/**
 * @brief Convert a Hex String to Char Array
*/
std::vector<uint8_t>  convert_HexStr2CharArray( std::string const& hexString );

/**
 * @brief Convert Hex String to Byte
*/
uint8_t HexString2Byte(const char* hex);



#endif
