/**
 * @file    CLICommandArgumentType.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#ifndef __CLI_CMD_CLICOMMANDARGUMENT_HPP__
#define __CLI_CMD_CLICOMMANDARGUMENT_HPP__

// C++ Standard Libraries
#include <cinttypes>
#include <string>

namespace CLI{
namespace CMD{

/**
 * @enum CLICommandArgument
*/
enum class CLICommandArgumentType : uint8_t {
    UNKNOWN = 0,
    STRING  = 1,
    INTEGER = 2,
    FLOAT   = 3,
}; // End of CLICommandArgument Enumeration


/**
 * @brief Convert a CLICommandArgumentType to String
*/
std::string CLICommandArgumentTypeToString( const CLICommandArgumentType& arg_type );


/**
 * @brief Convert a String to CLICommandArgumentType
 */
CLICommandArgumentType  StringToCLICommandArgumentType( const std::string& arg_type );


} // End of CMD Namespace
} // End of CLI Namespace

#endif
