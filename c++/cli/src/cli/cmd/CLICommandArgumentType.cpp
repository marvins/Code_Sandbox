/**
 * @file    CLICommandArgumentType.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "CLICommandArgumentType.hpp"

namespace CLI{
namespace CMD{

/*********************************************/
/*        Convert Argtype to string          */
/*********************************************/
std::string CLICommandArgumentTypeToString( CLICommandArgumentType const& arg_type )
{
    // String
    if( arg_type == CLICommandArgumentType::STRING ){
        return "STRING";
    }

    // Int8
    if( arg_type == CLICommandArgumentType::INT8 ){
        return "INT8";
    }


    return "UNKNOWN";
}


/*********************************************/
/*        Convert String to Argtype          */
/*********************************************/
CLICommandArgumentType  StringToCLICommandArgumentType( const std::string& arg_type )
{

    // String Type
    if( arg_type == "string" ||
        arg_type == "STRING" )
    {
        return CLICommandArgumentType::STRING;
    }

    // Integer Type
    if( arg_type == "int8" ||
        arg_type == "INT8" )
    {
        return CLICommandArgumentType::INT8;
    }

    return CLICommandArgumentType::UNKNOWN;
}


} // End of CMD Namespace
} // End of CLI Namespace


