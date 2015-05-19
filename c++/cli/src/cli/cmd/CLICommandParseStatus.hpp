/**
 * @file    CLICommandParseStatus.hpp
 * @author  Marvin Smith
 * @date    5/19/2015
*/
#ifndef __CLI_CMD_CLI_COMMAND_PARSE_STATUS_HPP__
#define __CLI_CMD_CLI_COMMAND_PARSE_STATUS_HPP__

// C++ Standard Libraries
#include <cinttypes>
#include <string>


namespace CLI{
namespace CMD{

/**
 * @enum CLICommandParseStatus
 */
enum class CLICommandParseStatus : uint8_t {
    UNKNOWN           = 0 /**< Unknown problem. Avoid this setting.*/,
    VALID             = 1 /**< Valid command with valid arguments.*/,
    NO_COMMAND_FOUND  = 2 /**< No command with a matching name found.*/,
}; // End of CLICommandParseStatus Enumeration


/**
 * @brief Convert a CLICommandParseStatus to String
 */
std::string CLICommandParseStatusToString( const CLICommandParseStatus& status );


/**
 * @brief Convert a string to CLICommandParseStatus
 */
CLICommandParseStatus StringToCLICommandParseStatus( std::string const& status );


} // End of CMD Namespace
} // End of CLI Namespace

#endif
