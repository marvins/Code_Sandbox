/**
 * @file    A_CLI_Manager_Factory
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_MANAGER_FACTORY_HPP__
#define __CLI_A_CLI_MANAGER_FACTORY_HPP__

// CLI Libraries
#include "A_CLI_Manager.hpp"

namespace CLI{

/**
 * @class A_CLI_Manager_Factory
 */
class A_CLI_Manager_Factory{


    public:
        
        /**
         * @brief Initialize
         *
         * @param[in] config_pathname
         */
        static A_CLI_Manager::ptr_t  Initialize( const std::string& config_pathname );



}; // End of A_CLI_Manager_Factory

} // End of CLI Namespace

#endif
