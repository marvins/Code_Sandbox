/**
 * @file    A_CLI_Command_Argument.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#ifndef __CLI_CMD_A_CLI_COMMAND_ARGUMENT_HPP__
#define __CLI_CMD_A_CLI_COMMAND_ARGUMENT_HPP__

// CLI Libraries
#include "CLICommandArgumentType.hpp"

namespace CLI{
namespace CMD{

/**
 * @class A_CLI_Command_Argument
*/
class A_CLI_Command_Argument{

    public:
        
        /**
         * @brief Default Constructor
        */
        A_CLI_Command_Argument();


        /**
         * @brief Constructor
        */
        A_CLI_Command_Argument( const std::string&             arg_name,
                                const CLICommandArgumentType&  arg_type,
                                const std::string&             arg_description,
                                const bool&                    arg_required = true,
                                const std::string&             arg_default_value = "" );

        /**
         * @brief Get the Name
         */
        inline std::string Get_Name()const{
            return m_name;
        }


        /**
         * @brief Get the Argument Type
         */
        inline CLICommandArgumentType Get_Type()const{
            return m_type;
        }


        /**
         * @brief Get the description
        */
        inline std::string Get_Description()const{
            return m_description;
        }
        

        /**
         * @brief Is Valid Type.
         */
        bool Is_Valid_Type( const std::string& test_str )const;


    private:
        
        /// Argument Name
        std::string m_name;

        /// Type
        CLICommandArgumentType m_type;

        /// Description
        std::string m_description;

        /// Default Value
        std::string m_default_value;

        /// Required
        bool m_required;

}; // End of A_CLI_Command_Argument Class

} // End of CMD Namespace
} // End of CLI Namespace

#endif
