/**
 * @file    Options.hpp
 * @author  Marvin Smith
 * @date    2/14/2016
*/
#ifndef __CPP_UTILITIES_ANALYSIS_UTILITY_OPTIONS_HPP__
#define __CPP_UTILITIES_ANALYSIS_UTILITY_OPTIONS_HPP__

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
        */
        Options( int argc, char* argv[] );
        

        /**
         * @brief Get the Test Image Path
        */
        inline std::string Get_Test_Image_Path()const{
            return m_test_image_path;
        }


        /**
         * @brief Get the Reference Image Path
        */
        inline std::string Get_Ref_Image_Path()const{
            return m_ref_image_path;
        }
        

        /**
         * @brief Print Usage
        */
        void Usage();

    private:

        /**
         * @brief Parse the Command-Line
        */
        void Parse_Command_Line( int argc, char* argv[] );

        /// Class Name
        std::string m_class_name;

        /// Application Name
        std::string m_program_name;

        /// Test Image Path
        std::string m_test_image_path;

        /// Reference Image Path
        std::string m_ref_image_path;

}; // End of Options Class


#endif
