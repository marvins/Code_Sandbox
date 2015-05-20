/**
 * @file    A_Ping_Command_Response_Handler.cpp
 * @author  Marvin Smith
 * @date    5/20/2015
 */
#include "A_Ping_Command_Response_Handler.hpp"

// C++ Standard Libraries
#include <iostream>

/**********************************/
/*          Constructor           */
/**********************************/
A_Ping_Command_Response_Handler::A_Ping_Command_Response_Handler()
  : CLI::A_Command_Response_Handler_Base()
{
}


/****************************************/
/*         Check if Supported           */
/****************************************/
bool A_Ping_Command_Response_Handler::Is_Supported( CLI::CMD::A_CLI_Command_Result const& result ) const
{
    return true;
}

/****************************************/
/*          Process the Command         */
/****************************************/
void A_Ping_Command_Response_Handler::Process_Command( CLI::CMD::A_CLI_Command_Result const& response ){

    std::cout << "Processing Command: " << response.To_Debug_String() << std::endl;

}


