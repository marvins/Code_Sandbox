/**
 * @file    Parser.hpp
 * @author  Marvin Smith
 * @date    2/21/2014
 */
#ifndef __SRC_CORE_PARSER_HPP__
#define __SRC_CORE_PARSER_HPP__

#include <string>
#include <utility>
#include <vector>

namespace PSR{

/**
 * @class Parser
 *
 *  A generic file parsing and config file loading class
*/
class Parser{

    public:
        
        /**
         * Default Constructor
        */
        Parser( );
        
        /**
         * Parameterized Constructor given a specific configuration filename.
        */
        Parser( std::string const& filename );
        
        /**
         * Parameterized Constructor given a list of command-line options and config filename.
        */
        Parser( const int argc, char ** argv, std::string const& filename );

        void init( const int argc, char ** argv, std::string const& filename );

        //string handling
        std::string getItem_string( const std::string& tag_name, bool& found )const;
        void   setItem_string( const std::string& tag_name, const std::string& value );
        void   setItem_string( const std::string& tag_name, const std::string& value, const bool& create );
        
        //boolean handling
        bool  getItem_bool( const std::string& tag_name, bool& found )const;
        void  setItem_bool( const std::string& tag_name, const bool& value );
        void  setItem_bool( const std::string& tag_name, const bool& value, const bool& create );

        //int handling
        int   getItem_int( const std::string& tag_name, bool& found )const;
        void  setItem_int( const std::string& tag_name, const int& value );
        void  setItem_int( const std::string& tag_name, const int& value, const bool& create );
        
        //double handling
        double getItem_double( const std::string& tag_name, bool& found )const;
        void   setItem_double( const std::string& tag_name, const double& value );
        void   setItem_double( const std::string& tag_name, const double& value, const bool& create );
        
        //double list handling
        std::vector<double> getItem_vec_double( const std::string& tag_name, bool& found )const;
        void                setItem_vec_double( const std::string& tag_name, const std::vector<double>& value );
        void                setItem_vec_double( const std::string& tag_name, const std::vector<double>& value, const bool& create );
        
        // list handling
        std::vector<std::string> getItem_vec_string( const std::string& tag_name, bool& found )const;
        void                     setItem_vec_string( const std::string& tag_name, const std::vector<std::string>& value );
        void                     setItem_vec_string( const std::string& tag_name, const std::vector<std::string>& value, const bool& create );
        
        /**
         * Add common flag
         */
        void addFlag( const std::string& flag );

        /**
         * Get flag
         */
        bool checkFlag( const std::string& flag )const;

        /**
         * Check to see if a file exists
         * 
         * @param[in] filename Name of the file
        */
        static bool fileExists( const std::string& filename );

    private:
        
        /** 
         * Validate that the configuration file given works.
        */
        void validate_file( );

        /**
         * Load the configuration file and read all contents.
        */
        void load_file();


        std::vector<std::pair<std::string,bool> > m_commonFlags; /*< Common Flags */
        std::string config_filename;                             /*< Configuration Filename */
        std::vector<std::pair<std::string,std::string> > items;  /*< List of contents. */
        bool valid;                                              /*< Validity Flag */
        std::string command_name;                                /*< Name of program */


}; // End of Parser class


} // End of PSR Namespace 

#endif
