/**
 * @file    message_service_demo.cpp
 * @author  Marvin Smith
 * @date    4/19/2017
*/

// Project Libraries
#include "MessageService.hpp"
#include "Utilities.hpp"

// C++ Libraries
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <unistd.h>


class DemoBase
{
    public:

        DemoBase(MessageService::ptr_t message_service)
          : m_class_name("DemoBase"),
            m_is_running(false),
            m_message_service(message_service)
        {
            LOG("End of Method");
        }

        virtual void Initialize()
        {
            // Create Runner Thread
            m_is_running = true;
            m_run_thread = std::thread( &DemoBase::Run, this);

            LOG("End of Method");
        }

        virtual void Wait_Complete()
        {
            LOG("Start of Method");
            m_is_running = false;
            if( m_run_thread.joinable() )
            {
                m_run_thread.join();
            }
            LOG("End of Method");
        }

        virtual void Finalize()
        {
            LOG("End of Method");

            if( m_is_running )
            {
                Wait_Complete();
            }
        }

    protected:

        // Run Thread
        virtual void Run() = 0;

        /// Message Service Instance
        MessageService::ptr_t m_message_service;

    private:

        /// Class Name
        std::string m_class_name;

        bool m_is_running;

        std::thread m_run_thread;

};

class Demo_User_01 : public DemoBase
{
    public:

        /**
         * @brief Constructor
        */
        Demo_User_01( MessageService::ptr_t message_service )
         : DemoBase( message_service ),
           m_class_name("Demo_User_01")
        {
            // Log Entry
            LOG("Start of Method.");

            // Subscribe
            std::function<void(std::string,std::string)> handler = std::bind(&Demo_User_01::Handle_Message, this, std::placeholders::_1, std::placeholders::_2);
            m_message_service->Subscribe( "TOPIC_02", handler );

            // Log Exit
            LOG("End of Method");
        }


        /**
         * @brief Handle a Message
        */
        void Handle_Message( std::string topic_name,
                             std::string message )
        {
            LOG("Received Message.  Topic: " + topic_name + ", Message: " + message);
        }

    protected:

        /**
         * @brief Run Method
        */
        virtual void Run()
        {
            // Log Entry
            LOG("Start of Method");

            // Send a Topic Message to Service
            m_message_service->Send( "TOPIC_01", "Hello From Demo_User_01" );

            LOG("End of Method");
        }

    private:

        /// Class Name
        std::string m_class_name;
};

class Demo_User_02 : public DemoBase
{
    public:

        /**
         * @brief Constructor
        */
        Demo_User_02( MessageService::ptr_t message_service )
         : DemoBase(message_service),
           m_class_name("Demo_User_02")
        {
            // Log Entry
            LOG("Start of Method.");

            // Subscribe
            std::function<void(std::string,std::string)> handler = std::bind(&Demo_User_02::Handle_Message, this, std::placeholders::_1, std::placeholders::_2);
            m_message_service->Subscribe( "TOPIC_01", handler );

            // Log Exit
            LOG("End of Method");
        }

        /**
         * @brief Handle a Message
        */
        void Handle_Message( std::string topic_name,
                             std::string message )
        {
            LOG("Received Message.  Topic: " + topic_name + ", Message: " + message);
        }

    protected:

        /**
         * @brief Run Method
        */
        virtual void Run()
        {
            LOG("Start of Method");

            LOG("End of Method");
        }

    private:

        /// Class Name
        std::string m_class_name;
};

int main( int argc, char* argv[] )
{
    // Build Message Service Configuration
    auto ms_config = std::make_shared<MessageServiceConfig>();

    // Create and Start Message Service
    auto message_service = std::make_shared<MessageService>( ms_config );

    message_service->Initialize();


    // Create Message-Service Users
    Demo_User_01 demo_user_01(message_service);
    Demo_User_02 demo_user_02(message_service);

    sleep(2);
    std::cout << "\n\n\n";
    std::cout << "Finished Initialization" << std::endl;

    // Setup
    demo_user_01.Initialize();
    demo_user_02.Initialize();

    demo_user_01.Wait_Complete();
    demo_user_02.Wait_Complete();

    demo_user_01.Finalize();
    demo_user_02.Finalize();

    // Kill Message Service
    message_service->Finalize();

    return 0;
}
