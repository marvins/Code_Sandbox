/**
 * @file    MessageService.cpp
 * @author  Marvin Smith
 * @date    4/19/2017
*/
#include "MessageService.hpp"

// C++ Libraries
#include <iostream>
#include <sstream>


/****************************************/
/*             Constructor              */
/****************************************/
MessageServiceConfig::MessageServiceConfig()
  : m_class_name("MessageServiceConfig")
{

}

/****************************************/
/*             Constructor              */
/****************************************/
MessageService::MessageService( MessageServiceConfig::ptr_t config )
  : m_class_name("MessageService"),
    m_config(config),
    m_is_initialized(false),
    m_listen_state(false)
{

}


/****************************************/
/*              Destructor              */
/****************************************/
MessageService::~MessageService()
{
    // Check if initialized
    if( Is_Initialized() )
    {
        Finalize();
    }
}

/****************************************/
/*             Initialize               */
/****************************************/
void MessageService::Initialize()
{
    // Start the queue
    m_queue.start();

    // Set the listen flag
    m_listen_state = true;
    m_listen_thread = std::thread( &MessageService::Run_Listener, this);

    // If No Errors, set to true
    m_is_initialized = true;
}

/****************************************/
/*               Finalize              */
/****************************************/
void MessageService::Finalize()
{
    // Log Entry
    LOG("Start of Method");

    // Set status and close thread
    m_listen_state = false;
    m_queue.complete();
    if( m_listen_thread.joinable() )
    {
        m_listen_thread.join();
    }

    // Note that we are no longer initialized
    m_is_initialized = false;

    // Log Exit
    LOG("End of Method");
}

/*******************************************/
/*           Subscribe a Listener          */
/*******************************************/
void MessageService::Subscribe( const std::string& topic_name,
                                HANDLER_TYPE handler_method )
{
    // Log
    LOG( "Subscribing Handler to Topic: " + topic_name);
    
    // Add to List of Subscribers
    m_subscribers[topic_name].push_back(handler_method);
}


/*********************************/
/*          Send Message         */
/*********************************/
void MessageService::Send( const std::string& topic_name,
                           const std::string& message )
{
    // Add to Queue
    m_queue.push( std::make_tuple(topic_name, message ));


}


/********************************/
/*         Run Listener         */
/********************************/
void MessageService::Run_Listener()
{
    std::tuple<std::string,std::string> tp;

    while( m_listen_state )
    {

        // Wait to pop
        m_queue.pop( tp );

        // Note released
        LOG( "Released from queue: " + std::get<0>(tp) + ", " + std::get<1>(tp));

        // Check if Topic is in list of Subscribers
        if( m_subscribers.find(std::get<0>(tp)) == m_subscribers.end() )
        {

            std::stringstream sin;
            sin << "Available Topics Registered\n";
            for( auto topic : m_subscribers){
                sin << topic.first << std::endl;
            }
            LOG("No Subscribers Registered to Topic: " + std::get<0>(tp) + "\n" + sin.str());
        }

        // Otherwise, pass to all handlers
        else
        {
            std::deque<HANDLER_TYPE> handlers = m_subscribers.find(std::get<0>(tp))->second;
            for( HANDLER_TYPE handler : handlers )
            {
                handler(std::get<0>(tp),
                        std::get<1>(tp));
            }
        }
    }
}
