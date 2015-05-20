/**
 * @file    ANSI_Utilities.hpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#ifndef __CLI_UTILS_ANSI_UTILITIES_HPP__
#define __CLI_UTILS_ANSI_UTILITIES_HPP__

// C++ Standard Libraries
#include <string>


namespace UTILS{

/// ANSI Escape Codes
const std::string ANSI_CLEARSCREEN = "\033[2J";
const std::string ANSI_RESETCURSOR = "\033[;H";

/// ANSI Color Codes
const std::string ANSI_RESET = "\033[m";
const std::string ANSI_GREEN = "\033[32m";

} // End of UTILS Namespace

#endif
