/**
 * @file    Options.cpp
 * @author  Marvin Smith
 * @date    1/21/2019
 */
#include "Options.hpp"

// C++ Libraries
#include <deque>
#include <iostream>
#include <fstream>
#include <string>

// Project Libraries
#include "String_Utilities.hpp"


namespace bf=boost::filesystem;


/********************************/
/*          Constructor         */
/********************************/
Options::Options(int argc, char **argv)
{
    Parse_Command_Line(argc, argv);
    Parse_Config_File();
    Log_Configuration();
}


/********************************************/
/*          Log the Configuration           */
/********************************************/
void Options::Log_Configuration()
{
    std::cout << "Configuration" << std::endl;
    std::cout << "=============" << std::endl;
    for( auto key : m_settings )
    {
        std::cout << "   - " << key.first << " = " << key.second << std::endl;
    }
}


/****************************************/
/*          Get the file list           */
/****************************************/
std::map<int,bf::path> Options::Get_File_List()const
{
    std::map<int,bf::path> output;
    bool found;

    int num_frames = Get_Setting_Int("NUMBER_IMAGES", found);
    if( !found )
    {
        throw std::runtime_error("No key (NUMBER_IMAGES) found");
    }

    for( int i=1; i<=num_frames; i++ )
    {
        std::string key = "IMAGE_PATH_" + std::to_string(i);
        auto path = Get_Setting_Path(key, found);
        if( !found ){
            throw std::runtime_error("Key: " + key + ", Not Found");
        }

        output[i] = path;
    }

    return output;
}


/****************************************************/
/*          Get the Setting as an Integer           */
/****************************************************/
int Options::Get_Setting_Int( const std::string& key,
                              bool&              found ) const
{
    auto str_result = Get_Setting( key, found );
    if( !found ){
        return 0;
    }
    return std::stoi(str_result);
}


/************************************************/
/*          Get the Setting as a Path           */
/************************************************/
boost::filesystem::path Options::Get_Setting_Path( const std::string&  key,
                                                   bool&               found ) const
{
    auto str_result = Get_Setting( key, found );
    if( !found ){
        return boost::filesystem::path();
    }
    return boost::filesystem::path(str_result);
}


/************************************************/
/*          Get the Setting as String           */
/************************************************/
std::string Options::Get_Setting( const std::string& key,
                                  bool&              found )const
{
    found = false;
    if( m_settings.find(key) == m_settings.end()){
        return "";
    }

    found = true;
    return m_settings.find(key)->second;
}


/************************************************/
/*          Parse Command-Line Options          */
/************************************************/
void Options::Parse_Command_Line(int argc, char **argv)
{
    m_program_name = argv[0];

    // Create arg list
    std::deque<std::string> args;
    for( int i=1; i<argc; i++ ){
        args.push_back(argv[i]);
    }

    // Parse Args
    std::string arg;
    while( !args.empty() )
    {
        arg = args.front();
        args.pop_front();

        if( arg == "-h" || arg == "--help" ){
            std::cerr << "Usage: " << m_program_name << " <args> -c <config-path>" << std::endl;
            std::exit(1);
        }
        else if( arg == "-c" ){
            m_config_path = bf::path(args.front());
            args.pop_front();
        }
        else{
            std::cerr << "error: unknown argument (" << arg << ")" << std::endl;
            std::exit(1);
        }
    }
}


/************************************************/
/*          Parse Configuration File            */
/************************************************/
void Options::Parse_Config_File()
{
    // Make sure file exists
    if( !bf::exists(m_config_path)){
        std::cerr << "Config path (" << m_config_path.native() << ") does not exist." << std::endl;
        std::exit(1);
    }

    // Open the file
    std::ifstream fin;
    fin.open(m_config_path.native().c_str());

    std::vector<std::string> parts;
    std::string line, pline;
    fin >> line;

    while( fin.good() )
    {
        // make sure it isn't a command
        pline = String_Trim( line );
        if( pline.empty() || pline[0] == '#' ){

        }

        // Parse, Strip, then Split the string
        else
        {
            parts = String_Split(pline, "=");

            if (parts.size() == 2) {
                m_settings[parts[0]] = parts[1];
            }
        }
        fin >> line;
    }

    fin.close();
}
