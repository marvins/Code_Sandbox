#include "String_Utilities.hpp"

/**
 * @brief Convert a Hex String to Char Array
*/
std::vector<uint8_t>  convert_HexStr2CharArray( std::string const& hexString ){
    
    // Create output
    std::vector<uint8_t> output;
    
    // Iterate over array
    for( int idx=0; idx<hexString.size(); idx+=2 ){
        output.push_back(HexString2Byte(hexString.substr(idx,2).c_str()));
    }

    return output;
}


/**
 * @brief Convert Hex String to Byte
*/
uint8_t HexString2Byte(const char* hex){

    unsigned short byte = 0;
    std::istringstream iss(hex);
    iss >> std::hex >> byte;
    return byte % 0x100;

}

