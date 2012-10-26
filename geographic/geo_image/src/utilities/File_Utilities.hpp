#ifndef __UTILITIES_FILEUTILITIES_HPP__
#define __UTILITIES_FILEUTILITIES_HPP__

#include <string>

namespace GEO{

namespace STR{


/**
 * Check to see if a file exists
 *
 * @param[in] filename Name of file to check existence for
 *
 * @return True if it exists, false otherwise
*/
bool file_exists( std::string const& filename );


/**
 * Return the basename of a file
 *
 * @param[in] filename name of the item to be resolved
 * 
 * @return base filename
*/
std::string file_basename( const std::string& filename );

/**
 * Return the extension of a file
 *
 * @param[in] filename of the item to be resolved
 *
 * @return Extension
*/
std::string file_extension( const std::string& filename );

} // end of STR Namespace
} // end of GEO Namespace

#endif

