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
         * @brief Create NCurses Context.
         *
         * @return NCurses Context.
         */
         NCURSES::An_NCurses_Context::ptr_t Create_NCurses_Context()const;
         

    private:
        
        /// Class Name
        std::string m_class_name;

        /// CLI Communication Type
        CLIConnectionType m_cli_conn_type;

        /// Connection Handler Configuration
        A_CLI_Connection_Handler_Base_Config::ptr_t m_connection_handler_configuration;

}; // End of A_CLI_Manager_Configuration Class


} // End of CLI Namespace

#endif
