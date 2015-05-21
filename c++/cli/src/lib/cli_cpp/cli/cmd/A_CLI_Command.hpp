/**
 * @file    A_CLI_Command.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
*/
#ifndef __CLI_CMD_A_CLI_COMMAND_HPP__
#define __CLI_CMD_A_CLI_COMMAND_HPP__

// C++ Standard Libraries
#include <string>
#include <vector>

// CLI Arguments
#include "A_CLI_Command_Argument.hpp"


namespace CLI{
namespace CMD{

/**
 * @class A_CLI_Command
*/
class A_CLI_Command{

    public:
        
        /**
         * @brief Default Constructor
        */
        A_CLI_Command();


        /**
         * @brief Constructor
        */
        A_CLI_Command( const std::string&                         command_name,
                       const std::string&                         command_description,
                       const bool&                                command_expect_response );

        /**
         * @brief Constructor
         */
        A_CLI_Command( const std::string&                         command_name,
                       const std::string&                         command_description,
                       const bool&                                command_expect_response,
                       const std::vector<A_CLI_Command_Argument>& command_arguments );


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


        /**
         * @brief Check if a response is required.
         */
        inline bool Response_Expected()const{
            return m_expect_response;
        }


        /**
         * @brief Get the argument list.
         *
         * @return Argument list.
         */
        inline std::vector<A_CLI_Command_Argument> Get_Argument_List()const
        {
            return m_command_argument_list;
        }
        

        /**
         * @brief Get the requested argument.
         *
         * @param[in] index Position to fetch.
         *
         * @return Desired command.
         */
        inline A_CLI_Command_Argument Get_Command_Argument( const int& index )const{
            return m_command_argument_list[index];
        }

        
        /**
         * @brief Check an argument against the list.
         */
        bool Check_Argument( const int& argument_idx,
                             const std::string& test_argument )const;
                        

        /**
         * @brief Check if Name Match
         */
        bool Is_Name_Match( const std::string& command_name )const;


    private:

        /// Command Name
        std::string m_command_name;

        /// Command Description
        std::string m_command_description;

        /// Command Argument List
        std::vector<A_CLI_Command_Argument> m_command_argument_list;
        
        /// Expect Response Flag
        bool m_expect_response;

}; // End of A_CLI_Command 

} // End of CMD Namespace
} // End of CLI Namespace

#endif
