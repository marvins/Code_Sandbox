/**
 * @file    A_CLI_Manager.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_MANAGER_HPP__
#define __CLI_A_CLI_MANAGER_HPP__

// CLI Libraries
#include "A_CLI_Manager_Configuration.hpp"

// C++ Standard Libraries
#include <memory>

namespace CLI{

/**
 * @class A_CLI_Manager
 */
class A_CLI_Manager{

    public:

        /// Pointer Type
        typedef std::shared_ptr<A_CLI_Manager> ptr_t;

        
        /**
         * @brief Constructor
         *
         * @param[in] configuration
         */
        A_CLI_Manager( const A_CLI_Manager_Configuration& configuration );

        
        /**
         * @brief Connect the CLI Manager
         */
        void Connect();


        /**
         * @brief Disconnect the CLI Manager
         */
        void Disconnect();

    private:
        
        /// Class Name
        std::string m_class_name;

        /// Configuration
        A_CLI_Manager_Configuration m_configuration;



}; // End of A_CLI_Manager Class

} // End of CLI Namespace

#endif
