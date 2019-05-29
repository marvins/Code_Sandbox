/**
 * @file    Options.hpp
 * @author  Marvin Smith
 * @date    5/23/2019
 */
#ifndef STAB_CHECKER_OPTIONS_HPP
#define STAB_CHECKER_OPTIONS_HPP

// C++ Libraries
#include <map>
#include <memory>


class Options
{
    public:

        typedef std::shared_ptr<Options> ptr_t;

        /**
         * @brief Parse the Command-Line Options
         * @param argc
         * @param argv
         * @return
         */
        static Options::ptr_t Parse_Command_Line( int argc, char* argv[] );

        /**
         * @brief Print usage instructions
         */
        static void Usage( const std::string& program_name );

        /**
         * @brief Get a requested setting, throw if not available.
         */
        std::string Get_Setting( const std::string& name )const;

        int Get_Setting_Int( const std::string& name )const;


        void Parse_Config_File( const std::string& pathname );

        std::string To_Log_String()const;


    private:

        std::map<std::string,std::string> m_settings;

};

#endif