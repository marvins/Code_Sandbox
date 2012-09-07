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
        Parser( );
        Parser( std::string const& filename );
        
        //string handling
        std::string getItem_string( const std::string& tag_name, bool& found )const;
        void   setItem_string( const std::string& tag_name, const std::string& value );
        void   setItem_string( const std::string& tag_name, const std::string& value, const bool& create );

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


    private:
        
        void validate_file( );

        void load_file();

        std::string config_filename;
    
        std::vector<std::pair<std::string,std::string> > items;
        
        bool valid;

}; // End of Parser class


} // End of PSR Namespace 

#endif
