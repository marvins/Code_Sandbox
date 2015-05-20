/**
 * @file    A_CLI_Command_Parser.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#ifndef __CLI_CMD_A_CLI_COMMAND_PARSER_HPP__
#define __CLI_CMD_A_CLI_COMMAND_PARSER_HPP__

// C++ Standard Libraries
#include <memory>
#include <string>
#include <vector>

// CLI Libraries
#include "A_CLI_Command.hpp"
#include "A_CLI_Command_Result.hpp"
#include "A_CLI_Parser_Command.hpp"


namespace CLI{
namespace CMD{

/**
 * @class A_CLI_Command_Parser
*/
class A_CLI_Command_Parser {

    public:

        /// Pointer Type
        typedef std::shared_ptr<A_CLI_Command_Parser> ptr_t;

        
        /**
         * @brief Constructor
        */
        A_CLI_Command_Parser( const std::string&                        regex_split_pattern,
                              std::vector<A_CLI_Parser_Command> const&  parser_command_list,
                              std::vector<A_CLI_Command> const&         command_list );

        
        /**
         * @brief Evaluate Command
         *
         * @param[in] test_str String to evaluate.
         *
         * @return Command result.
         */
        A_CLI_Command_Result Evaluate_Command( const std::string& test_str ) const;
    
    
    private:
    
        /**
         * @brief Parse the string.
         */
        std::vector<std::string> Parse_String( const std::string& test_str )const;


        /// Class Name
        std::string m_class_name;

        /// Command List
        std::vector<A_CLI_Command> m_command_list;

        /// Parser Command List
        std::vector<A_CLI_Parser_Command> m_parser_command_list;

        /// Regex Split Pattern
        std::string m_regex_split_pattern;

}; // End of A_CLI_Command_Parser Class

} // End of CMD Namespace
} // End of CLI Namespace

#endif
