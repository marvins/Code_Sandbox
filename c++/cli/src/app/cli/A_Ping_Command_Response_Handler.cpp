/**
 * @file    A_Ping_Command_Response_Handler.cpp
 * @author  Marvin Smith
 * @date    5/20/2015
 */
#include "A_Ping_Command_Response_Handler.hpp"

// Demo Libraries
#include "../utils/System_Utilities.hpp"

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
void A_Ping_Command_Response_Handler::Process_Command( CLI::CMD::A_CLI_Command_Result::ptr_t response ){

    // Define our values
    std::string  hostname   = response->Get_Argument_Value<std::string>( 0 );
    int max_attempts        = response->Get_Argument_Value<int>( 1 );
    std::string details;


    // Run the command
    bool result = Ping( hostname, max_attempts,  details );

    if( result == true ){
        response->Set_System_Response( hostname + " is responding.");
    }
    else{
        response->Set_System_Response( hostname + " is not responding. Details: " + details );
    }
    
}


