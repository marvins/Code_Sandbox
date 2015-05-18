/**
 * @file    A_CLI_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Manager.hpp"


// CLI Libraries


// C++ Standard Libraries



namespace CLI{


/********************************/
/*          Constructor         */
/********************************/
A_CLI_Manager::A_CLI_Manager( A_CLI_Manager_Configuration const& configuration )
  : m_class_name("A_CLI_Manager"),
    m_configuration(configuration),
    m_ncurses_context(NULL)
{


}


/**********************************************/
/*          Connect the CLI Manager           */
/**********************************************/
void A_CLI_Manager::Connect()
{
    // Initialize NCurses
    NCURSES::Initialize( m_ncurses_context );

}



/**********************************************/
/*        Disconnect the CLI Manager          */
/**********************************************/
void A_CLI_Manager::Disconnect()
{
    // Finalize NCurses
    NCURSES::Finalize( m_ncurses_context );

}


} // End of CLI Namespace

