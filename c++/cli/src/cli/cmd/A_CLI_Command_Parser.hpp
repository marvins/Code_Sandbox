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
        A_CLI_Command_Parser();


    private:
        
        /// Class Name
        std::string m_class_name;


}; // End of A_CLI_Command_Parser Class

} // End of CMD Namespace
} // End of CLI Namespace

#endif
