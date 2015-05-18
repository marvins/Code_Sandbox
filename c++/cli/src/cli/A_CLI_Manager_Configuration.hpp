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
        A_CLI_Manager_Configuration( CLIConnectionType const& cli_comm_type );

    
        /**
         * @brief Get the CLI Communication Type
         *
         * @return CLI Communication Type
        */
        inline CLIConnectionType Get_CLI_Communication_Type()const{
            return m_cli_comm_type;
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
        CLIConnectionType m_cli_comm_type;


}; // End of A_CLI_Manager_Configuration Class


} // End of CLI Namespace

#endif
