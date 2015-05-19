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
        A_CLI_Command_Parser( std::vector<A_CLI_Command> const& command_list );


    private:
        
        /// Class Name
        std::string m_class_name;

        /// Command List
        std::vector<A_CLI_Command> m_command_list;

}; // End of A_CLI_Command_Parser Class

} // End of CMD Namespace
} // End of CLI Namespace

#endif
