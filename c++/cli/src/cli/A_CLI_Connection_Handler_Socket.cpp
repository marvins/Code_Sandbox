/**
 * @file    A_CLI_Connection_Handler_Socket.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_CLI_Connection_Handler_Socket.hpp"

// CLI Libraries
#include "A_CLI_Connection_Handler_Socket_Config.hpp"

// Reference: http://www.linuxhowtos.org/C_C++/socket.htm

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
    m_socket_fd = socket( AF_INET, SOCK_STREAM, 0 );
    if( m_socket_fd < 0 ){
        std::cerr << "error opening socket" << std::endl;
        return;
    }

    // configure the endpoint
    struct sockaddr_in serv_addr, cli_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    
    // Get the port number
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = m_configuration->Get_Port();

    // Bind the socket
    if( bind( m_socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0 ){
        std::cerr << "error binding socket" << std::endl;
        return;
    }

    // Listen



    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[256];
    int n;
    
    
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
                sizeof(serv_addr)) < 0) 
        error("ERROR on binding");
    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, 
            (struct sockaddr *) &cli_addr, 
            &clilen);
    if (newsockfd < 0) 
        error("ERROR on accept");
    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if (n < 0) error("ERROR reading from socket");
    printf("Here is the message: %s\n",buffer);
    n = write(newsockfd,"I got your message",18);
    if (n < 0) error("ERROR writing to socket");
    close(newsockfd);
    close(sockfd);

}


/***********************************************/
/*          Run the message handler            */
/***********************************************/
void A_CLI_Connection_Handler_Socket::Run_Handler()
{




}


} // End of CLI Namespacee

