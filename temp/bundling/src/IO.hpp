#ifndef __SRC_IO_HPP__
#define __SRC_IO_HPP__

#include <algorithm>
#include <deque>
#include <sstream>
#include <string>

#include "Options.hpp"

const int  IO_FILES_ONLY = 1;
const int  IO_DIRECTORIES_ONLY = 2;
const int  IO_ALL = 3;


/**
 * @class camlist_sort
 *
 * This is a simple functor which will sort two strings
 * assuming that they are camera CAM_IDs.
*/
class camlist_sort{

    public:

        bool operator() ( string const& a, string const& b ){
            return (a.substr(a.size()-4) < b.substr(b.size()-4));
        }

};

/**
 * @class ImageBundle
*/
class ImageBundle{

    public:
    
        /**
         * Default Constructor
        */
        ImageBundle();
        
        /**
         * Parameterized Constructor
        */
        ImageBundle( const std::string& imagename, const int& collectType );
        
        int scene_number;
        std::deque<std::string> data;

};


/**
 * Output stream operator for the ImageBundle Object
*/
std::ostream& operator << ( std::ostream& ostr, ImageBundle const& bundle );


/**
 * Query the computer for a date/time stamp
*/
std::string query_datetime();

/**
 * Create a zip file
*/
void compress_bundles( std::deque<ImageBundle> const& bundles, Options const& options );

void directory_append_internal( const std::string& filename, std::deque<std::string>& container, const int& flag = IO_ALL );

/**
 * Create the directory specified by dirname
 *
 * @param[in] dirname
 *
*/
void directory_create( std::string const& dirname );

std::string file_basename( const std::string& pathname );

std::string file_extension( const std::string& pathname );

/** 
 * Pop the right-most item from the path string. 
*/
std::string file_pop_rear( std::string const& filename );

std::deque<std::string> file_decompose_path( std::string const& pathname );

std::string file_merge_path( std::deque<std::string> const& pathname );

bool is_dir(  const std::string& name );
bool is_file( const std::string& name );

bool file_exists( std::string const& filename );

void file_copy( std::string const& filename, std::string const& dest_filename );

void file_delete( std::string const& filename );

void sort_image_list( std::deque<std::string>& image_list );



/**************************************************************************/
/*                           String Utilities                             */
/**************************************************************************/

/**
 * Convert a hex string into a decimal number
*/
template <typename TP>
TP hexstr2decnum( std::string const& value ){
    
    std::string data = value;
    std::transform( data.begin(), data.end(), data.begin(), ::tolower );

    std::stringstream sin;
    TP result;
    sin << std::hex << data;
    sin >> result;
    return result;
}
/**
 * Convert a string into a number
*/
template <typename TP>
TP str2num( std::string const& value ){
    
    std::stringstream sin;
    TP result;
    sin << value;
    sin >> result;
    return result;
}

/**
 * Convert a number into a string
 *
 * @param[in] value The value to convert
 *
 * @return String representation
 *
 * @note Has only been tested with doubles, floats, ints, uints
*/
template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}

/**
 * Trim a string
*/
std::string string_trim( const string& str );

/**
 * Split a string
*/
std::vector<std::string> string_split( const std::string& str, const std::string& pattern );

/**
 * Convert a string to lower
*/
std::string string_toLower( const string& str );

#endif

