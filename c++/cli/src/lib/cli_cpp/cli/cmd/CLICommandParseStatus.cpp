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

    // Back help
    if( status == CLICommandParseStatus::CLI_BACK ){
        return "CLI_BACK";
    }

    // Invalid arguments
    if( status == CLICommandParseStatus::INVALID_ARGUMENTS ){
        return "INVALID_ARGUMENTS";
    }

    // Excessive Arguments
    if( status == CLICommandParseStatus::EXCESSIVE_ARGUMENTS ){
        return "EXCESSIVE_ARGUMENTS";
    }

    // No Command Found
    if( status == CLICommandParseStatus::NO_COMMAND_FOUND ){
        return "NO_COMMAND_FOUND";
    }

    // UNKNOWN
    return "UNKNOWN";
}


/********************************************************/
/*      Convert CLI Command Parse Status to String      */
/********************************************************/
std::string CLICommandParseStatusToHistoryString( CLICommandParseStatus const& status )
{

    // Valid
    if( status == CLICommandParseStatus::VALID ){
        return "Valid Command";
    }

    // Valid Shutdown
    if( status == CLICommandParseStatus::CLI_SHUTDOWN ){
        return "CLI Shutdown Command";
    }

    // Valid help
    if( status == CLICommandParseStatus::CLI_HELP ){
        return "Success";
    }

    // Back help
    if( status == CLICommandParseStatus::CLI_BACK ){
        return "Success";
    }

    // Invalid arguments
    if( status == CLICommandParseStatus::INVALID_ARGUMENTS ){
        return "Invalid arguments";
    }

    // Excessive Arguments
    if( status == CLICommandParseStatus::EXCESSIVE_ARGUMENTS ){
        return "Too many arguments";
    }

    // No Command Found
    if( status == CLICommandParseStatus::NO_COMMAND_FOUND ){
        return "No command found.";
    }

    // UNKNOWN
    return "Unknown";
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

    // Excessive Arguments
    if( status == "EXCESSIVE_ARGUMENTS" ){
        return CLICommandParseStatus::EXCESSIVE_ARGUMENTS;
    }

    // No Command Found
    if( status == "NO_COMMAND_FOUND" ){
        return CLICommandParseStatus::NO_COMMAND_FOUND;
    }

    // Back Command
    if( status == "CLI_BACK" ){
        return CLICommandParseStatus::CLI_BACK;
    }

    // Unknown
    return CLICommandParseStatus::UNKNOWN;
}


/************************************************************/
/*      Check CLI Command Status if Valid CLI Command       */
/************************************************************/
bool Is_Valid_CLI_Command( CLICommandParseStatus const& command ){

    // Check if Valid
    switch( command ){
        
        // Valid Commands
        case CLICommandParseStatus::CLI_BACK:
        case CLICommandParseStatus::CLI_HELP:
        case CLICommandParseStatus::CLI_SHUTDOWN:
            return true;

    }
    return false;
}

} // End of CMD Namespace
} // End of CLI Namespace

