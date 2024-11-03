#ifndef __UTILITIES_HPP__
#define __UTILITIES_HPP__

// C++ Libraries
#include <fstream>
#include <iostream>
#include <string>


/**
 * @brief Load the J2K Data.
 */
void Load_J2K_Data( const std::string&  input_path,
                    uint8_t*&           input_buffer,
                    int64_t&            input_size );
    

#endif
