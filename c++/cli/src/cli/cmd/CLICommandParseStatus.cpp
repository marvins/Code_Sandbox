/**
 * @file    CLICommandParseStatus.cpp
 * @author  Marvin Smith
 * @date    5/19/2015
*/
#include "CLICommandParseStatus.hpp"

namespace CLI{
namespace CMD{

/********************************************************/
/*      Convert CLI Command Parse Status to String      */
/********************************************************/
std::string CLICommandParseStatusToString( CLICommandParseStatus const& status )
{

    // Valid
    if( status == CLICommandParseStatus::VALID ){
        return "VALID";
    }

    // Valid Shutdown
    if( status == CLICommandParseStatus::CLI_SHUTDOWN ){
        return "CLI_SHUTDOWN";
    }

    // Valid help
    if( status == CLICommandParseStatus::CLI_HELP ){
        return "CLI_HELP";
    }

    // Invalid arguments
    if( status == CLICommandParseStatus::INVALID_ARGUMENTS ){
        return "INVALID_ARGUMENTS";
    }

    // No Command Found
    if( status == CLICommandParseStatus::NO_COMMAND_FOUND ){
        return "NO_COMMAND_FOUND";
    }

    // UNKNOWN
    return "UNKNOWN";
}


/********************************************************/
/*      Convert String to CLI Command Parse Status      */
/********************************************************/
CLICommandParseStatus StringToCLICommandParseStatus( const std::string& status )
{
    // Valid Status
    if( status == "VALID" ){
        return CLICommandParseStatus::VALID;
    }

    // Valid Shutdown
    if( status == "CLI_SHUTDOWN" ){
        return CLICommandParseStatus::CLI_SHUTDOWN;
    }    

    // Valid help
    if( status == "CLI_HELP" ){
        return CLICommandParseStatus::CLI_HELP;
    }

    // Invalid ARguments
    if( status == "INVALID_ARGUMENTS" ){
        return CLICommandParseStatus::INVALID_ARGUMENTS;
    }

    // No Command Found
    if( status == "NO_COMMAND_FOUND" ){
        return CLICommandParseStatus::NO_COMMAND_FOUND;
    }

    // Unknown
    return CLICommandParseStatus::UNKNOWN;
}


} // End of CMD Namespace
} // End of CLI Namespace

