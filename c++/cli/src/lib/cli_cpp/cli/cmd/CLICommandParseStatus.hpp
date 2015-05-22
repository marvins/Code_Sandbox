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
    UNKNOWN             = 0 /**< Unknown problem. Avoid this setting.*/,
    VALID               = 1 /**< Valid command with valid arguments.*/,
    NO_COMMAND_FOUND    = 2 /**< No command with a matching name found.*/,
    CLI_SHUTDOWN        = 3 /**< The user set the parser's expected shutdown command.*/,
    CLI_HELP            = 4 /**< The user requested help */,
    INVALID_ARGUMENTS   = 5 /**< Problem parsing arguments.*/,
    EXCESSIVE_ARGUMENTS = 6 /**< Too many arguments compared to matching command.*/,
    CLI_BACK            = 7 /**< Back command entered.*/,
}; // End of CLICommandParseStatus Enumeration

/**
 * @brief Convert a CLICommandParseStatus to Color Code
 */
int CLICommandParseStatusToColorCode( const CLICommandParseStatus& status );

/**
 * @brief Convert a CLICommandParseStatus to String
 */
std::string CLICommandParseStatusToString( const CLICommandParseStatus& status );


/**
 * @brief Convert a CLICommandParseStatus to History Mode String
 */
std::string CLICommandParseStatusToHistoryString( const CLICommandParseStatus& status );


/**
 * @brief Convert a string to CLICommandParseStatus
 */
CLICommandParseStatus StringToCLICommandParseStatus( std::string const& status );


/**
 * @brief Check if CLI Command Status is a Valid CLI Command
 *
 * @param[in] command Command to test.
 *
 * @return True if a valid command, false otherwise.
*/
bool Is_Valid_CLI_Command( CLICommandParseStatus const& command );


} // End of CMD Namespace
} // End of CLI Namespace

#endif
