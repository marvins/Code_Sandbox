/**
 * @file    A_CLI_Connection_Handler_Socket.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Connection_Handler_Socket.hpp"

// CLI Libraries
#include "A_CLI_Connection_Handler_Socket_Config.hpp"

namespace CLI{


/*************************/
/*      Constructor      */
/*************************/
A_CLI_Connection_Handler_Socket::A_CLI_Connection_Handler_Socket( A_CLI_Connection_Handler_Base_Config::ptr_t configuration )
  : A_CLI_Connection_Handler_Base(),
    m_class_name("A_CLI_Connection_Handler_Socket")
{
    // Cast the configuration
    m_configuration = std::dynamic_pointer_cast<A_CLI_Connection_Handler_Socket_Config>(configuration);
}


/*****************************/
/*       Destructor          */
/*****************************/
A_CLI_Connection_Handler_Socket::~A_CLI_Connection_Handler_Socket()
{

}


/***********************************************/
/*          Run the message handler            */
/***********************************************/
void A_CLI_Connection_Handler_Socket::Run_Handler()
{




}


} // End of CLI Namespacee

