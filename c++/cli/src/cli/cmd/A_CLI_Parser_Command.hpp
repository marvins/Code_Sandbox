/**
 * @file    A_CLI_Parser_Command.hpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#ifndef __CLI_CMD_A_CLI_PARSER_COMMAND_HPP__
#define __CLI_CMD_A_CLI_PARSER_COMMAND_HPP__


// C++ Standard Libraries
#include <string>
#include <vector>

// CLI Libraries
#include "CLICommandParseStatus.hpp"

namespace CLI{
namespace CMD{


/**
 * @class A_CLI_Parser_Command
 */
class A_CLI_Parser_Command{

    public:
        
        /**
         * @brief Constructor
         */
        A_CLI_Parser_Command( const CLICommandParseStatus& mode );

        
        /**
         * @brief Get the mode
         */
        inline CLICommandParseStatus Get_Mode()const{
            return m_mode;
        }


        /**
         * @brief Add Name
         */
        void Add_Name( const std::string& name );


        /**
         * @brief Check for Match
         */
        bool Is_Match( const std::string& name )const;

    
    private:
        
        /// Mode
        CLICommandParseStatus m_mode;

        /// Names
        std::vector<std::string> m_names;

}; // End of A_CLI_Parser_Command Class


} // End of CMD Namespace
} // End of CLI Namespace

#endif
