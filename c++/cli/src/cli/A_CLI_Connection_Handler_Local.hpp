/**
 * @file    A_CLI_Connection_Handler_Local.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#ifndef __CLI_A_CLI_CONNECTION_HANDLER_LOCAL_HPP__
#define __CLI_A_CLI_CONNECTION_HANDLER_LOCAL_HPP__

// CLI Libraries
#include "A_CLI_Connection_Handler_Base.hpp"
#include "A_CLI_Connection_Handler_Local_Config.hpp"

// C++ Standard Libraries
#include <memory>

namespace CLI{

/**
 * @class A_CLI_Connection_Handler_Local
 */
class A_CLI_Connection_Handler_Local : public A_CLI_Connection_Handler_Base
{

    public:
        
        /// Pointer Type
        typedef std::shared_ptr<A_CLI_Connection_Handler_Local> ptr_t;

        
        /**
         * @brief Constructor
        */
        A_CLI_Connection_Handler_Local( A_CLI_Connection_Handler_Local_Config::ptr_t configuration );


        /**
         * @brief Destructor
         */
        ~A_CLI_Connection_Handler_Local();


    private:
        
        /// Class Name
        std::string m_class_name;

        /// Configuration
        A_CLI_Connection_Handler_Config::ptr_t  m_configuration;

}; // End of A_CLI_Connection_Handler_Local Class

} // End of CLI Namespace

#endif
