/**
 * @file    A_CLI_Manager_Configuration.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_MANAGER_CONFIGURATION_HPP__
#define __CLI_A_CLI_MANAGER_CONFIGURATION_HPP__

// C++ Standard Libraries
#include <string>

namespace CLI{


/**
 * @class A_CLI_Manager_Configuration
 */
class A_CLI_Manager_Configuration{

    public:
        
        /**
         * @brief Constructor
         */
        A_CLI_Manager_Configuration();




    private:
        
        /// Class Name
        std::string m_class_name;


}; // End of A_CLI_Manager_Configuration Class


} // End of CLI Namespace

#endif
