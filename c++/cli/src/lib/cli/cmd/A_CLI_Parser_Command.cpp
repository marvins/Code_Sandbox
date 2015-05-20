/**
 * @file    A_CLI_Parser_Command.cpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#include "A_CLI_Parser_Command.hpp"


namespace CLI{
namespace CMD{


/***********************************/
/*          Constructor            */
/***********************************/
A_CLI_Parser_Command::A_CLI_Parser_Command( const CLICommandParseStatus& mode )
  : m_mode(mode)
{
}


/************************************/
/*              Add Name            */
/************************************/
void A_CLI_Parser_Command::Add_Name( const std::string& name )
{
    m_names.push_back( name );
}


/****************************/
/*          Is Valid        */
/****************************/
bool A_CLI_Parser_Command::Is_Match( const std::string& name )const
{
    for( size_t i=0; i<m_names.size(); i++ ){
        if( m_names[i] == name ){
            return true;
        }
    }
    return false;
}


} // End of CMD Namespace
} // End of CLI Namespace


