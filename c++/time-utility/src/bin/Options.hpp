/**
 * @file    Options.hpp
 * @author  Marvin Smith
 * @date    10/19/2015
 */
#ifndef __TIME_UTILITY_OPTIONS_HPP__
#define __TIME_UTILITY_OPTIONS_HPP__

// C++ Libraries
#include <string>

// Time Utility Libraries
#include "Globals.hpp"


/**
 * @class Options
 *
 * @brief Store Command-Line and other configuration information.
 */
class Options
{
    public:
        
        /**
         * @brief Constructor
         *
         * @param[in] argc
         * @param[in] argv
         */
        Options( int argc, char* argv[] );
    

        /**
         * @brief Get the Program Name.
         *
         * @return Program Name.
         */
        inline std::string Get_Program_Name()const{
            return m_program_name;
        }
        

        /**
         * @brief Get the Program Mode
         *
         * @return Program Mode.
         */
        inline ProgramMode Get_Program_Mode()const{
            return m_program_mode;
        }


        /**
         * @brief Print Usage Instructions.
         */
        void Usage()const;


    private:

        /**
         * @brief Parse the Command-Line.
         *
         * @param[in] argc
         * @param[in] argv
         */
        void Parse_Command_Line( int argc, char* argv[] );


        /// Class Name
        std::string m_class_name;

        
        /// Application Name
        std::string m_program_name;

        
        /// Program Mode
        ProgramMode m_program_mode;

}; // End of Options Class


#endif
