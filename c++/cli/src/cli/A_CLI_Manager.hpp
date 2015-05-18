/**
 * @file    A_CLI_Manager.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_MANAGER_HPP__
#define __CLI_A_CLI_MANAGER_HPP__

namespace CLI{

/**
 * @class A_CLI_Manager
 */
class A_CLI_Manager{

    public:
        
        /**
         * @brief Constructor
         *
         * @param[in] configuration
         */
        A_CLI_Manager( const A_CLI_Manager_Configuration& configuration );


    private:
        
        /// Class Name
        std::string m_class_name;

        /// Configuration
        A_CLI_Manager_Configuration m_configuration;



}; // End of A_CLI_Manager Class

} // End of CLI Namespace

#endif
