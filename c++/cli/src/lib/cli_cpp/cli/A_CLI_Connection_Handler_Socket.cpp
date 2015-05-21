/**
 * @file    A_CLI_Connection_Handler_Socket.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Connection_Handler_Socket.hpp"

// CLI Libraries
#include "A_CLI_Connection_Handler_Socket_Config.hpp"
#include "A_Console_Render_Manager_ASCII.hpp"

// C++ Standard Libraries
#include <cstring>
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const std::string KEYBOARD_DELETE_KEY = "\033\133\063\176";

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

    // Configure the socket
    Setup_Socket();
}


/*****************************/
/*       Destructor          */
/*****************************/
A_CLI_Connection_Handler_Socket::~A_CLI_Connection_Handler_Socket()
{

}


/********************************************/
/*          Configure the SOcket            */
/********************************************/
void A_CLI_Connection_Handler_Socket::Setup_Socket()
{

    // Setup the socket
    m_sock_fd = socket( AF_INET, SOCK_STREAM, 0 );
    if( m_sock_fd < 0 ){
        std::cerr << "error opening socket" << std::endl;
        return;
    }

    // Configure the socket
    int optionFlag = 1;
    if( setsockopt( m_sock_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&optionFlag, sizeof(int)) < 0 ){
        std::cerr << "error setting SO_REUSEADDR" << std::endl;
        return;
    }

    // configure the endpoint
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    // Get the port number
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(m_configuration->Get_Port());

    // Bind the socket
    if( bind( m_sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 ){
        std::cerr << "error binding socket" << std::endl;
        return;
    }

    // Listen
    if( listen( m_sock_fd, 5 ) < 0 ){
        std::cerr << "error listening on socket." << std::endl;
        return;
    }

}


/********************************/
/*       Close the socket       */
/********************************/
void A_CLI_Connection_Handler_Socket::Close_Socket()
{
    close( m_sock_fd );
    close( m_client_fd );
}


/***********************************************/
/*          Run the message handler            */
/***********************************************/
void A_CLI_Connection_Handler_Socket::Run_Handler()
{

    // Misc Variables
    int key;

    // Read the socket
    char buffer[256];
    int n;
    std::string input;
    
    
    // Get the length
    socklen_t clilen;
    struct sockaddr_in cli_addr;
    clilen = sizeof(cli_addr);

    // Accept the socket
    m_client_fd = accept( m_sock_fd, 
                          (struct sockaddr*)&cli_addr,
                          &clilen);

    if( m_client_fd < 0 ){
        std::cerr << "error accepting socket" << std::endl;
        return;
    }

    // Write back
    write( m_client_fd,"\377\375\042\377\373\001",6);
    write( m_client_fd,"Welcome\n\0", 9);

    // Setup NCUrses for the socket
    if( this->m_console_render_manager != nullptr ){
        this->m_console_render_manager->Initialize();
        this->m_render_state = this->m_console_render_manager->Get_Render_State();
    }
    
    // run until time to quit
    while( true ){
    
        // Check the manager
        if( this->m_console_render_manager == nullptr ){
            break;
        }


        
        // Check keyboard value
        n = read( m_client_fd, buffer, 255 );
        if( n == 0 ){
            std::cerr << "socket closed." << std::endl;
            break;
        }
        if (n < 0){
            std::cerr << "error reading the socket" << std::endl;
        }
        
        // Check the buffer
        input = std::string(buffer).substr(0,n);
        
        // Process the text
        if( input.size() > 1 ){
            
            // Check Delete Key
            if( input == KEYBOARD_DELETE_KEY ){
                this->m_render_state->Process_Input( KEY_DC );
            }
            
            
            // Otherwise, there was an error
            else{
                std::cerr << "Warning, data is larger than expected. Size: " << input.size() << std::endl;
                for( size_t i=0; i<input.size(); i++ ){
                    std::cout << i << " : " << (int)input[i] << std::endl;
                }
            }
        }
        else{
            
            // cast the key
            key = input[0];

            // Check if enter
            if( key == 27 || key == 13 || key == 10 ){
                this->Process_Command();
            }
            
            // Otherwise, add the key
            else{
                this->m_render_state->Process_Input( key );
            }
        }

        
        // Render the screen
        this->m_console_render_manager->Refresh();

        // Get the buffer string
        std::vector<std::string>& buffer_data = std::dynamic_pointer_cast<A_Console_Render_Manager_ASCII>(m_console_render_manager)->Get_Console_Buffer();
       
        // Write each line to the socket
        for( size_t i=0; i<buffer_data.size(); i++ ){
            write( m_client_fd, buffer_data[i].c_str(), buffer_data[i].size() );
        }

        // Check if time to exit
        if( m_is_running == false ){
            break;
        }

    }

    // Set the running flag
    m_is_running = false;

    // Close Socket
    Close_Socket();

}


} // End of CLI Namespacee

