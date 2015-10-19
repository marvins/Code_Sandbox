/**
 * @file    Globals.hpp
 * @author  Marvin Smith
 * @date    10/19/2015
 */
#ifndef __TIME_UTILITY_GLOBALS_HPP__
#define __TIME_UTILITY_GLOBALS_HPP__

// C++ Libraries
#include <cinttypes>


/**
 * @enum ProgramMode
 *
 * @brief Enumerates available program operation modes.
 */
enum class ProgramMode : uint8_t
{
    UNKNOWN     = 0 /**< Error condition.*/,
    PRINT_TIMES = 1 /**< Print time to stdout*/,
}; // End of ProgramMode Enumeration


#endif
