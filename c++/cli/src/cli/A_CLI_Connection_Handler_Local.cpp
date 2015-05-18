/**
 * @file    A_CLI_Connection_Handler_Local.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Connection_Handler_Local.hpp"

namespace CLI{


/*************************/
/*      Constructor      */
/*************************/
A_CLI_Connection_Handler_Local::A_CLI_Connection_Handler_Local( A_CLI_Connection_Handler_Base_Config::ptr_t configuration )
  : A_CLI_Connection_Handler_Base(),
    m_class_name("A_CLI_Connection_Handler_Local")
{
    
    // Cast the configuration
    m_configuration = std::dynamic_pointer_cast<A_CLI_Connection_Handler_Local_Config>(configuration);
}


/*****************************/
/*       Destructor          */
/*****************************/
A_CLI_Connection_Handler_Local::~A_CLI_Connection_Handler_Local()
{

}


/***********************************************/
/*          Run the message handler            */
/***********************************************/
void A_CLI_Connection_Handler_Local::Run_Handler()
{




}



} // End of CLI Namespacee

