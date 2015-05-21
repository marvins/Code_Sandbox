/**
 * @file    A_CLI_Command_Result.hpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#ifndef __CLI_CMD_A_CLI_COMMAND_RESULT_HPP__
#define __CLI_CMD_A_CLI_COMMAND_RESULT_HPP__

// CLI Libraries
#include "A_CLI_Command.hpp"
#include "CLICommandParseStatus.hpp"
#include "../../utils/String_Utilities.hpp"

// C++ Standard Libraries
#include <memory>
#include <string>
#include <type_traits>

namespace CLI{
namespace CMD{

/**
 * @class A_CLI_Command_Result
 */
class A_CLI_Command_Result{

    public:
        
        /// Pointer Type
        typedef std::shared_ptr<A_CLI_Command_Result> ptr_t;
        
        /**
         * @brief Constructor
        */
        A_CLI_Command_Result();


        /**
         * @brief Constructor
         *
         * @param[in] parse_status Parsing operation status.
         * @param[in] command Command that was executed.
         */
        A_CLI_Command_Result( CLICommandParseStatus const&    parse_status,
                              A_CLI_Command const&            command );
       

        /**
         * @brief Constructor
         *
         * @param[in] parse_status Parsing operation status.
         * @param[in] command Command that was executed.
         * @param[in] argument_values List of arguments provided to the command.
         */
        A_CLI_Command_Result( CLICommandParseStatus const&    parse_status,
                              A_CLI_Command const&            command,
                              std::vector<std::string> const& argument_values );

    
        /**
         * @brief Get the Parsing Status.
         *
         * @return Parse status.
         */
        inline CLICommandParseStatus Get_Parse_Status()const{
            return m_parse_status;
        }


        /**
         * @brief Get the Parsing Status
        */
        std::string Get_Parse_Status_String()const;


        /**
         * @brief Get the command.
         *
         * @return Command.
         */
        inline A_CLI_Command Get_Command()const{
            return m_command;
        }

    
        /**
         * @brief Process Command Results
         *
         * @param[in] command Command that was executed.
         * @param[in] arguments List of arguments to check for validity.
         *
         * @return Result of the operation.
         */
        static A_CLI_Command_Result  Process_Arguments( const A_CLI_Command&             command,
                                                        const std::vector<std::string>&  arguments );

        /**
         * @brief Print to a debug string.
         */
        std::string To_Debug_String( const int& offset = 0 )const;

        
        /**
         * @brief Set the response from the system.
         *
         * @param[in] response Response to set.
         */
        void Set_System_Response( const std::string& response );

        
        /**
         * @brief Get the system response.
         *
         * @return system response value
         */
        inline std::string Get_System_Response()const{
            return m_system_response_value;
        }


        /**
         * @brief Check system response.
         *
         * @return True if received, false otherwise.
         */
        inline bool Check_System_Response()const{
            return m_system_response_set;
        }
        

        /**
         * @brief Get argument value.
         *
         * @param[in] idx argument index.
         *
         * @return result Result value.
         */
        template <typename TP>
        typename std::enable_if<std::is_same<TP,std::string>::value,std::string>::type
        Get_Argument_Value( const int& idx )const
        {
            // Check if we need to use the default
            if( (int)m_argument_values.size() <= idx ){
                return m_command.Get_Command_Argument(idx).Get_Default_Value();
            }
            
            // Otherwise, return the actual argument
            return m_argument_values[idx];
        }

       
        /**
         * @brief Get argument value.
         *
         * @param[in] idx argument index.
         *
         * @return result Result value.
         */
        template <typename TP>
        typename std::enable_if<std::is_arithmetic<TP>::value,TP>::type
        Get_Argument_Value( const int& idx )const
        {
            // Check if we need to use the default
            if( (int)m_argument_values.size() <= idx ){
                return UTILS::str2num<TP>( m_command.Get_Command_Argument(idx).Get_Default_Value());
            }
            
            // Otherwise, return the actual argument
            return UTILS::str2num<TP>( m_argument_values[idx] );
        }



    private:
    
        /// Status
        CLICommandParseStatus m_parse_status;

        /// Command Found
        A_CLI_Command m_command;

        /// Argument Values
        std::vector<std::string> m_argument_values;

        /// System response
        bool m_system_response_set = false;

        /// Response 
        std::string m_system_response_value;

}; // End of A_CLI_Command_Result Class

} // End of CMD Namespace
} // End of CLI Namespace

#endif
