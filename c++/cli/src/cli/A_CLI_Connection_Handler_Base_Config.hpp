/**
 * @file    A_CLI_Connection_Handler_Base_Config.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_CONNECTION_HANDLER_BASE_CONFIG_HPP__
#define __CLI_A_CLI_CONNECTION_HANDLER_BASE_CONFIG_HPP__

// C++ Standard Libraries
#include <memory>
#include <string>

namespace CLI{

/**
 * @class A_CLI_Connection_Handler_Base_Config
 */
class A_CLI_Connection_Handler_Base_Config{

    public:
        
        /// Pointer Type
        typedef std::shared_ptr<A_CLI_Connection_Handler_Base_Config> ptr_t;

        /**
         * @brief Constructor
         */
        A_CLI_Connection_Handler_Base_Config();


        /**
         * @brief Destructor
         */
        virtual ~A_CLI_Connection_Handler_Base_Config();


    private:
        
        /// Class Name
        std::string m_class_name;

}; // End of A_CLI_Connection_Handler_Base_Config Class


} // End of CLI Namespace

#endif
