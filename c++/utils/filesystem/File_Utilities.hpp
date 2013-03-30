#ifndef __SRC_FILEUTILITIES_HPP__
#define __SRC_FILEUTILITIES_HPP__

#include <string>

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

#endif

