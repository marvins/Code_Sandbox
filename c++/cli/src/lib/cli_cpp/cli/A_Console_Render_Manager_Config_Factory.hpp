/**
 * @file    A_Console_Render_Manager_Config_Factory.hpp
 * @author  Marvin Smith
 * @date    5/21/2015
*/
#ifndef __CLI_A_CONSOLE_RENDER_MANAGER_CONFIG_FACTORY_HPP__
#define __CLI_A_CONSOLE_RENDER_MANAGER_CONFIG_FACTORY_HPP__

// CLI Libraries
#include "A_Console_Render_Manager_Config_Factory.hpp"


namespace CLI{


/**
 * @class A_Console_Render_Manager_Config_Factory
 */
class A_Console_Render_Manager_Config_Factory{

    public:
        
        /**
         * @brief Initialize
         */
        static A_Console_Render_Manager_Config::ptr_t Initialize( CLIConnectionType const& conn_type );


}; // End of A_Console_Render_Manager_Config_Factory


} // End of CLI Namespace

#endif
