/**
 * @file    A_CLI_Command.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#ifndef __CLI_CMD_A_CLI_COMMAND_HPP__
#define __CLI_CMD_A_CLI_COMMAND_HPP__

// C++ Standard Libraries
#include <string>

namespace CLI{
namespace CMD{

/**
 * @class A_CLI_Command
*/
class A_CLI_Command{

    public:
        
        /**
         * @brief Constructor
        */
        A_CLI_Command( const std::string& command_name,
                       const std::string& command_description );

        /**
         * @brief Get the Command Name.
         * 
         * @return Command name.
         */
        inline std::string Get_Name()const{
            return m_command_name;
        }


        /**
         * @brief Get the command description.
         */
        inline std::string Get_Description()const{
            return m_command_description;
        }


    private:

        /// Command Name
        std::string m_command_name;

        /// Command Description
        std::string m_command_description;

}; // End of A_CLI_Command 

} // End of CMD Namespace
} // End of CLI Namespace

#endif
