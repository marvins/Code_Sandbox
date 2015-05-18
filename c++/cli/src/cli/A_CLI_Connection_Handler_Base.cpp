/**
 * @file    A_CLI_Connection_Handler_Base.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#include "A_CLI_Connection_Handler_Base.hpp"

namespace CLI{


/************************/
/*      Constructor     */
/************************/
A_CLI_Connection_Handler_Base::A_CLI_Connection_Handler_Base()
  : m_is_running(false),
    m_class_name("A_CLI_Connection_Handler_Base")
{
}



/*************************/
/*       Destructor      */
/*************************/
A_CLI_Connection_Handler_Base::~A_CLI_Connection_Handler_Base()
{


}


/***************************************/
/*          Start the Handler          */
/***************************************/
void A_CLI_Connection_Handler_Base::Start_Handler()
{
    // Signal that we are running
    m_is_running = true;

    // Run the handler
    this->Run_Handler();

}


/*************************************/
/*          Stop the Handler         */
/*************************************/
void A_CLI_Connection_Handler_Base::Signal_Shutdown()
{
    // set the flag
    m_is_running = false;
}



} // End of CLI Namespace

