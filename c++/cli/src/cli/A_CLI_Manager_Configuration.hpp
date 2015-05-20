/**
 * @file    A_CLI_Manager_Configuration.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_MANAGER_CONFIGURATION_HPP__
#define __CLI_A_CLI_MANAGER_CONFIGURATION_HPP__

// C++ Standard Libraries
#include <string>

// CLI Libraries
#include "A_CLI_Connection_Handler_Base.hpp"
#include "A_CLI_Connection_Handler_Base_Config.hpp"
#include "CLIConnectionType.hpp"
#include "cmd/A_CLI_Command_Parser.hpp"
#include "../thirdparty/ncurses/NCurses_Utilities.hpp"

namespace CLI{


/**
 * @class A_CLI_Manager_Configuration
 */
class A_CLI_Manager_Configuration{

    public:
        
        /**
         * @brief Constructor
         */
        A_CLI_Manager_Configuration( CLIConnectionType const& cli_conn_type );

    
        /**
         * @brief Get the CLI Communication Type
         *
         * @return CLI Communication Type
        */
        inline CLIConnectionType Get_CLI_Connection_Type()const{
            return m_cli_conn_type;
        }


        /**
         * @brief Set the CLI Connection Type.
         *
         * @param[in] cli_conn_type CLI Connection Type.
         */
        inline void Set_CLI_Connection_Type( CLIConnectionType const& cli_conn_type )
        {
            m_cli_conn_type = cli_conn_type;
        }


        /**
         * @brief Get the CLI Connection Handler Configuration
         *
         * @return Connection Handler Configuration.
         */
        A_CLI_Connection_Handler_Base::ptr_t  Get_Connection_Handler()const;

        
        /**
         * @brief Set the CLI Connection Handler Configuration
         *
         * @param[in] configuration Configuration object to set.
         */
        inline void Set_CLI_Connection_Handler_Config( A_CLI_Connection_Handler_Base_Config::ptr_t configuration )
        {
            m_connection_handler_configuration = configuration;
        }


        /**
         * @brief Get the Command Parser
         * 
         * @return Command Parser.
         */
        inline CMD::A_CLI_Command_Parser::ptr_t Get_CLI_Command_Parser()const{
            return m_command_parser;
        }


        /**
         * @brief Set the Command Parser
        */
        inline void Set_CLI_Command_Parser( CMD::A_CLI_Command_Parser::ptr_t command_parser ){
            m_command_parser = command_parser;
        }


        /**
         * @brief Set the CLI Title.
         *
         * @return CLI Title
        */
        inline std::string Get_CLI_Title()const{
            return m_cli_title;
        }


        /**
         * @brief Set the CLI Title
         *
         * @param[in] cli_title
        */
        inline void Set_CLI_Title( const std::string& cli_title ){
            m_cli_title = cli_title;
        }


        /**
         * @brief Get the CLI Command Queue Max Size
        */
        inline int Get_CLI_Command_Queue_Max_Size()const{
            return m_cli_command_queue_max_size;
        }


        /**
         * @brief Set the CLI Command Queue Max Size
        */
        inline void Set_CLI_Command_Queue_Max_Size( const int& cli_command_queue_max_size )
        {
            m_cli_command_queue_max_size = cli_command_queue_max_size;
        }


        /**
         * @brief Create NCurses Context.
         *
         * @return NCurses Context.
         */
         NCURSES::An_NCurses_Context::ptr_t Create_NCurses_Context()const;
         

         /**
          * @brief Check if the configuration is valid.
          *
          * @return True if valid, false otherwise.
          */
         bool Is_Valid()const;


    private:
        
        /// Class Name
        std::string m_class_name;

        /// CLI Communication Type
        CLIConnectionType m_cli_conn_type;

        /// Connection Handler Configuration
        A_CLI_Connection_Handler_Base_Config::ptr_t m_connection_handler_configuration;

        /// Command Parser
        CMD::A_CLI_Command_Parser::ptr_t m_command_parser;
        
        /// Command-Line Title
        std::string m_cli_title;
            
        /// CLI Command Queue Max Size
        int m_cli_command_queue_max_size;


}; // End of A_CLI_Manager_Configuration Class


} // End of CLI Namespace

#endif
