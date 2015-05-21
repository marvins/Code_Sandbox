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

        
        /**
         * @brief Set the Formal Name
         */
        void Set_Formal_Name( const std::string& formal_name );


        /**
         * @brief Get the formal name
         */
        inline std::string Get_Formal_Name()const{
            return m_formal_name;
        }


        /**
         * @brief Get the Description.
         *
         * @return Description string.
         */
        inline std::string Get_Description()const{
            return m_description;
        }


        /**
         * @brief Set the description.
         *
         * @param[in] description Description to set.
         */
        inline void Set_Description( const std::string& description ){
            m_description = description;
        }


        /**
         * @brief Get the name list.
         * 
         * @return list of command names.
         */
        inline std::vector<std::string> Get_Command_Name_List()const{
            return m_names;
        }
        
    
    private:
        
        /// Mode
        CLICommandParseStatus m_mode;

        /// Names
        std::vector<std::string> m_names;

        /// Formal Name
        std::string m_formal_name;

        /// Description
        std::string m_description;

}; // End of A_CLI_Parser_Command Class


} // End of CMD Namespace
} // End of CLI Namespace

#endif
