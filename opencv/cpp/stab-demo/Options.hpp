/**
 * @file    Options.hpp
 * @author  Marvin Smith
 * @date    1/21/2019
 */
#ifndef OPENCV_STAB_DEMO_OPTIONS_HPP
#define OPENCV_STAB_DEMO_OPTIONS_HPP

// Boost Libraries
#include <boost/filesystem.hpp>

// C++ Libraries
#include <map>
#include <string>

class Options
{
    public:

        /**
         * @brief Constructor
         * @param argc
         * @param argv
         */
        Options( int argc, char* argv[] );

        void Log_Configuration();

        /**
         * @brief Get the file list.  Key is the frame-number.
         * @return
         */
        std::map<int,boost::filesystem::path> Get_File_List()const;


        /**
         * @brief Look for value
         * @param key
         * @param found
         * @return
         */
        int Get_Setting_Int( const std::string& key,
                             bool&              found )const;


        /**
         *
         * @param key
         * @param found
         * @return
         */
        boost::filesystem::path  Get_Setting_Path( const std::string& key,
                                                   bool&              found )const;

        /**
         *
         * @param key
         * @param found
         * @return
         */
        std::string  Get_Setting( const std::string& key,
                                  bool&              found )const;

    private:

        void Parse_Command_Line( int argc, char* argv[] );

        void Parse_Config_File();

        std::string m_program_name;

        boost::filesystem::path m_config_path;

        std::map<std::string,std::string> m_settings;
};

#endif