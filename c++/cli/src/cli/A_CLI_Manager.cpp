/**
 * @file    A_CLI_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Manager.hpp"


// CLI Libraries
#include "A_CLI_Connection_Handler_Base.hpp"

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

    // Create CLI Communication Type
    m_ncurses_context = m_configuration.Create_NCurses_Context();

    // Grab the connection handler
    m_connection_handler = m_configuration.Get_Connection_Handler();
}


/**********************************************/
/*          Connect the CLI Manager           */
/**********************************************/
void A_CLI_Manager::Connect()
{
    // Initialize NCurses
    NCURSES::Initialize( m_ncurses_context );
    
    // Kick off the console thread

    
    // Kick off the communication thread
    A_CLI_Connection_Handler_Base* handler_ptr = m_connection_handler.get();
    m_connection_thread = std::thread( &A_CLI_Connection_Handler_Base::Start_Handler,
                                       std::ref(handler_ptr));

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

