/**
 * @file    CLIConnectionType.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "CLIConnectionType.hpp"


namespace CLI{


/*******************************************/
/*      Convert a string to CLI type       */
/*******************************************/
CLIConnectionType  StringToCLIConnectionType( const std::string& cli_type )
{
    if( cli_type == "LOCAL" ){
        return CLIConnectionType::LOCAL;
    }

    if( cli_type == "SOCKET" ){
        return CLIConnectionType::SOCKET;
    }

    return CLIConnectionType::UNKNOWN;
}



/*************************************************/
/*      Convert a connection type to string      */
/*************************************************/
std::string CLIConnectionTypeToString( const CLIConnectionType& cli_type )
{
    if( cli_type == CLIConnectionType::LOCAL ){ return "LOCAL"; }
    if( cli_type == CLIConnectionType::SOCKET ){ return "SOCKET"; }
    return "UNKONWN";
}


} // End of CLI Namespace

