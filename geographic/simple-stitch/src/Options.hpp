/**
 * @file    Options.hpp
 * @author  Marvin Smith
 * @date    10/27/2015
*/
#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

// C++ Libraries
#include <string>

/**
 * @class Options
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
        Options( int   argc,
                 char* argv[] );

        
        /**
         * @brief Get the Base Directory.
         * 
         * @return Base directory.
        */
        inline std::string Get_Base_Directory()const{
            return m_base_directory;
        }
         

        /**
         * @brief Usage Instructions
        */
        void Usage();

    
    private:
        
        /// Class Name
        std::string m_class_name;

        /// Application Name
        std::string m_application_name;

        /// Base Directory
        std::string m_base_directory;


}; // End of Options Class


#endif
