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


    // Integer
    if( arg_type == CLICommandArgumentType::INTEGER ){
        return "INTEGER";
    }


    // Float
    if( arg_type == CLICommandArgumentType::FLOAT ){
        return "FLOAT";
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
    if( arg_type == "integer" ||
        arg_type == "INTEGER" )
    {
        return CLICommandArgumentType::INTEGER;
    }

    // Float Type
    if( arg_type == "float" || 
        arg_type == "FLOAT" )
    {
        return CLICommandArgumentType::FLOAT;
    }


    return CLICommandArgumentType::UNKNOWN;
}


} // End of CMD Namespace
} // End of CLI Namespace


