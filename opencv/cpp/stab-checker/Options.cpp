/**
 * @file    Options.cpp
 * @author  Marvin Smith
 * @date    5/23/2019
 */
#include "Options.hpp"

// C++ Libraries
#include <deque>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

// Project Libraries
#include "Log_Utilities.hpp"
#include "String_Utilities.hpp"

/********************************************/
/*          Parse the Command Line          */
/********************************************/
Options::ptr_t  Options::Parse_Command_Line(int argc, char **argv)
{
    LOG_TRACE("Parsing Command-Line");

    // Create Option Class
    auto options = std::make_shared<Options>();

    // Create list
    std::deque<std::string> args;
    std::string program_name = argv[0];
    std::string arg;
    for( int i=1; i<argc; i++ ){
        args.emplace_back(argv[i]);
    }

    while( !args.empty() )
    {
        // pop the next arg off
        arg = args.front();
        args.pop_front();

        // Iterate through list
        if( arg == "-h" || arg == "--help" )
        {
            Usage(program_name);
            std::exit(0);
        }

        // Get video name
        if( arg == "-v" )
        {
            options->m_settings["INPUT_VIDEO_PATH"] = args.front();
            args.pop_front();
        }

        // Get Configuration
        if( arg == "-c" )
        {
            options->Parse_Config_File( args.front() );
            args.pop_front();
        }
    }

    LOG_TRACE("Finished Building Configuration");
    LOG_TRACE(options->To_Log_String());

    return options;
}


/************************************************/
/*          Print Usage Instructions            */
/************************************************/
void Options::Usage( const std::string& program_name )
{
    std::cerr << "usage: " << program_name << " -v <video-path> -c <config> [other flags]" << std::endl;

}


/************************************************/
/*          Get the Requested Setting           */
/************************************************/
std::string Options::Get_Setting(const std::string &name) const
{
    if( m_settings.find(name) == m_settings.end() ){
        throw std::runtime_error("No setting [" + name + "] found.  Did you provide config file?");
    }
    return m_settings.find(name)->second;
}


/************************************************/
/*          Get the Requested Setting           */
/************************************************/
int Options::Get_Setting_Int(const std::string &name) const
{
    try{
        return std::stoi(Get_Setting(name));
    }
    catch(...)
    {
        return std::numeric_limits<int>::min();
    }
}


/****************************************************/
/*          Parse the configuration file            */
/****************************************************/
void Options::Parse_Config_File(const std::string &pathname)
{
    LOG_TRACE("Parsing Config File: " + pathname);
    std::fstream fin;
    fin.open(pathname.c_str());

    char tline[256];
    int len=256;
    std::string line;
    fin.getline(tline, len);
    line = tline;

    while( fin.good() )
    {
        LOG_TRACE("-> Next Line: " + line);
        // Trim and split the string
        auto trimmed_line = String_Trim(line);
        if( !trimmed_line.empty() && trimmed_line[0] != '#' )
        {
            auto split_line = String_Split(trimmed_line, "=");
            LOG_TRACE("Parts, " + std::to_string(split_line.size()));
            m_settings[String_Trim(split_line[0])] = String_Trim(split_line[1]);
        }

        // Get next entry
        fin.getline(tline, len);
        line = tline;
    }

    fin.close();
}


/********************************************/
/*          Print to Logging String         */
/********************************************/
std::string Options::To_Log_String() const
{
    std::stringstream sin;
    sin << "Configuration" << std::endl;
    for( const auto& p : m_settings )
    {
        sin << " - " << p.first << " : " << p.second << std::endl;
    }
    return sin.str();
}