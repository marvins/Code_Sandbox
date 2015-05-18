/**
 * @file    A_CLI_Configuration_File_Parser.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CLI_CONFIGURATION_FILE_PARSER_HPP__
#define __CLI_A_CLI_CONFIGURATION_FILE_PARSER_HPP__

// C++ Standard Libraries
#include <string>

// CLI Libraries
#include "A_CLI_Manager_Configuration.hpp"


namespace CLI{

/**
 * @class A_CLI_Configuration_File_Parser
 */
class A_CLI_Configuration_File_Parser{

    public:
        
        /**
         * @brief Constructor
         *
         * @param[in] config_path Path to configuration file.
         */
        A_CLI_Configuration_File_Parser( const std::string& config_path );
    
        
        /**
         * @brief Get the CLI Manager Configuration.
         *
         * @return CLI Manager Configuration Object.
         */
        A_CLI_Manager_Configuration Get_CLI_Manager_Configuration()const;


    private:
    
        /// Class Name
        std::string m_class_name;

        /// Configuration File Path
        std::string m_config_pathname;


}; // End of A_CLI_Configuration_File_Parser Class

} // End of CLI Namespace

#endif
