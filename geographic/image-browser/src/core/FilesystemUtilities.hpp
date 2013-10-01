/**
 * @file    FilesystemUtilities.hpp
 * @author  Marvin Smith 
 * @date    10/1/2013
 */
#ifndef __SRC_CORE_FILESYSTEMUTILITIES_HPP__
#define __SRC_CORE_FILESYSTEMUTILITIES_HPP__

#include <string>

/**
 * Check if a given filename is a real file
 *
 * @param[in] filename Filename of a file (absolute or relative will work)
 * @return True if exists, false otherwise
 */
bool file_exists( const std::string& filename );

/**
 * Return the file extension
 * 
 * @param[in] filename File to be evaluated.
 * @returns   File extension
*/
std::string file_extension( const std::string& filename );


/**
 * Return the filename of the object
 *
 * @param[in] filename path string to be evaluated.
 * @return filename component of path
*/
std::string file_basename( const std::string& filename );

/**
 * Return the cannonical name of the path
 */
std::string file_canonical( const std::string& filename );

/**
 * Return the current working directory
*/
std::string current_working_directory( );


#endif
