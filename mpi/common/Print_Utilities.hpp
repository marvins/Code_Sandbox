/**
 * @file    Print_Utilities.hpp
 * @author  Marvin Smith
 * @date    6/6/2018
 */
#ifndef COMMON_PRINT_UTILITIES_HPP
#define COMMON_PRINT_UTILITIES_HPP

// C++ Libraries
#include <string>


/**
 * @brief Print Useful MPI System Info
 */
void Print_System_Info( int rank,
                        int nprocs,
                        std::string hostname );

#endif

