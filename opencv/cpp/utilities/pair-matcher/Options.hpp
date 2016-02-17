/**
 * @file    Options.hpp
 * @author  Marvin Smith
 * @date    2/15/2016
*/
#ifndef __PAIR_MATCHER_OPTIONS_HPP__
#define __PAIR_MATCHER_OPTIONS_HPP__

// C++ Libraries
#include <map>
#include <string>

/**
 * @class Options
*/
class Options
{
    public:
        
        /**
         * @brief Constructor
        */
        Options( int argc, char* argv[] );


        /**
         * @brief Get the Program Name
        */
        inline std::string Get_Program_Name()const{
            return m_program_name;
        }


        /**
         * @brief Print Usage Instruction
        */
        void Usage();
        

        /**
         * @brief Get Config Setting
        */
        inline std::string Get_Config_Parameter( const std::string& key )const{
            if( m_config_settings.find(key) == m_config_settings.end() ){
                return "";
            }
            return m_config_settings.find(key)->second;
        }

    private:
        
        /**
         * @brief Parse the Command-Line Options
        */
        void Parse_Command_Line( int argc, char* argv[] );
        

        /// Class Name
        std::string m_class_name;


        /// Program Name
        std::string m_program_name;


        /// Key/Value Pair List
        std::map<std::string,std::string> m_config_settings;

}; // End of Options class


#endif
