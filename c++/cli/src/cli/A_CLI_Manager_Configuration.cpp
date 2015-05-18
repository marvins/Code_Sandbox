/**
 * @file    A_CLI_Manager_Configuration.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Manager_Configuration.hpp"

namespace CLI{


/***************************/
/*      Constructor        */
/***************************/
A_CLI_Manager_Configuration::A_CLI_Manager_Configuration( CLIConnectionType const& cli_conn_type )
  : m_class_name("A_CLI_Manager_Configuration"),
    m_cli_conn_type(cli_conn_type)
{
}


/************************************/
/*       Create NCurses Context     */
/************************************/
NCURSES::An_NCurses_Context::ptr_t  A_CLI_Manager_Configuration::Create_NCurses_Context()const
{
    
    // Create new object
    NCURSES::An_NCurses_Context::ptr_t context = std::make_shared<NCURSES::An_NCurses_Context>();


    // Attach communication file pointers
    if( m_cli_conn_type == CLIConnectionType::LOCAL ){
        context->tty_in  = stdin;
        context->tty_out = stdout;
    }
    else{
        return nullptr;
    }


    // return the new context
    return context;
}



} // End of CLI Namespace
