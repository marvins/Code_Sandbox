/**
 * @file    MessageService.hpp
 * @author  Marvin Smith
 * @date    4/17/2017
*/
#ifndef MESSAGE_SERVICE_HPP
#define MESSAGE_SERVICE_HPP

// C++ Libraries
#include <deque>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <thread>
#include <tuple>


// Project Libraries
#include "BlockingQueue.hpp"
#include "Utilities.hpp"


/**
 * @class MessageServiceConfig
*/
class MessageServiceConfig
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<MessageServiceConfig> ptr_t;

        /**
         * @brief Constructor
        */
        MessageServiceConfig();

    private:

        /// Class Name
        std::string m_class_name;

}; // End of MessageService Class

/**
 * @class MessageService
*/
class MessageService
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<MessageService> ptr_t;

        /// Handler Type
        typedef std::function<void(std::string, std::string)> HANDLER_TYPE;

        /**
         * @brief Constructor
        */
        MessageService( MessageServiceConfig::ptr_t config );

        /**
         * @brief Destructor
        */
        virtual ~MessageService();


        /**
         * @brief Initialize Message Service
        */
        void Initialize();


        /**
         * @brief Finalize Message Service
        */
        void Finalize();


        /**
         * @brief Register a Subscriber
        */
        void Subscribe( const std::string& topic_name,
                        HANDLER_TYPE handler_method );

        /**
         * @brief Send Message
         *
         * @brief topic_name Topic to publish on
         * @brief message   Text to send
        */
        void Send( const std::string& topic_name,
                   const std::string& message );


        /**
         * @brief Check if initialized
        */
        inline bool Is_Initialized()const{
            return m_is_initialized;
        }

    protected:

        /**
         * @brief Run Listener
        */
        void Run_Listener();

    private:

        /// Class Name
        std::string m_class_name;

        /// Configuration
        MessageServiceConfig::ptr_t m_config;

        /// Is initialized
        bool m_is_initialized;

        /// Blocking Queue
        Blocking_Queue<std::tuple<std::string,std::string>> m_queue;

        /// Processing Thread
        std::thread m_listen_thread;

        /// State
        bool m_listen_state;


        /// Subscriber List
        std::map<std::string,std::deque<HANDLER_TYPE>> m_subscribers;

}; // End of MessageService Class

#endif
