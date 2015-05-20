/**
 * @file    CLIConnectionType.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_CLICONNECTIONTYPE_HPP__
#define __CLI_CLICONNECTIONTYPE_HPP__

// C++ Standard Libraries
#include <string>

namespace CLI{

/**
 * @enum CLIConnectionType
 */
enum class CLIConnectionType : uint8_t {
    UNKNOWN = 0,
    LOCAL   = 1,
    SOCKET  = 2,
}; // End of CLIConnectionType Enum


/**
 * @brief Convert String to CLI Connection Type.
 *
 * @param[in] cli_type CLI Type string.
 *
 * @return Connection Type
 */
CLIConnectionType StringToCLIConnectionType( std::string const& cli_type );


/**
 * @brief Convert CLIConnectionType to a string.
 *
 * @param[in] cli_type CLI Connection Type.
 *
 * @return String format.
 */
std::string CLIConnectionTypeToString( const CLIConnectionType& cli_type );


} // End of CLI Namespace

#endif
