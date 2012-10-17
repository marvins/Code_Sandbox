#ifndef __SRC_IO_HPP__
#define __SRC_IO_HPP__

#include <deque>
#include <sstream>
#include <string>

#include "Options.hpp"

const int  IO_FILES_ONLY = 1;
const int  IO_DIRECTORIES_ONLY = 2;
const int  IO_ALL = 3;


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
        ImageBundle( const std::string& imagename );
        
        int scene_number;
        std::deque<std::string> data;

};


std::ostream& operator << ( std::ostream& ostr, ImageBundle const& bundle );



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

bool is_dir(  const std::string& name );
bool is_file( const std::string& name );

void file_copy( std::string const& filename, std::string const& dest_filename );


void sort_image_list( std::deque<std::string>& image_list );


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


#endif

