/**
 * @file    A_CLI_Connection_Handler_Base.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_CONNECTION_HANDLER_BASE_HPP__
#define __CLI_A_CLI_CONNECTION_HANDLER_BASE_HPP__

// C++ Standard Libraries
#include <memory>
#include <string>
#include <thread>

// CLI Libraries
#include "A_Console_Render_Manager.hpp"
#include "cmd/A_CLI_Command_Parser.hpp"

namespace CLI{

/**
 * @class A_CLI_Connection_Handler_Base
*/
class A_CLI_Connection_Handler_Base{

    public:
        
        /// Pointer type
        typedef std::shared_ptr<A_CLI_Connection_Handler_Base> ptr_t;


        /**
         * @brief Constructor
         */
        A_CLI_Connection_Handler_Base();


        /**
         * @brief Destructor
        */
        virtual ~A_CLI_Connection_Handler_Base();

        
        /**
         * @brief Start the handler
         */
        virtual void Start_Handler();


        /**
         * @brief Stop the handler
         */
        virtual void Signal_Shutdown();

        
        /**
         * @brief Wait for shutdown
         */
        virtual void Wait_Shutdown();

        
        /**
         * @brief Update the Registered Rendering Manager
         */
        inline virtual void Update_Render_Manager( A_Console_Render_Manager::ptr_t render_manager )
        {
            m_console_render_manager = render_manager;
            m_render_state = m_console_render_manager->Get_Render_State();
        }


        /**
         * @brief Update the Registered Command Parser.
         */
        inline virtual void Update_CLI_Command_Parser( CMD::A_CLI_Command_Parser::ptr_t cli_command_parser )
        {
            m_cli_command_parser = cli_command_parser;
        }


    protected:
        
        /**
         * @brief Run the handler
         */
        virtual void Run_Handler() = 0;


        /// Running Flag
        bool m_is_running;


        /// Console Render Manager
        A_Console_Render_Manager::ptr_t m_console_render_manager;

        
        /// Current Render State
        A_Console_Render_State::ptr_t m_render_state;

        /// CLI Command Parser
        CMD::A_CLI_Command_Parser::ptr_t m_cli_command_parser;

    private:
        
        /// Class Name
        std::string m_class_name;

        /// Thread
        std::thread m_connection_thread;

}; // End of A_CLI_Connection_Handler_Base Class

} // End of CLI Namespace

#endif
