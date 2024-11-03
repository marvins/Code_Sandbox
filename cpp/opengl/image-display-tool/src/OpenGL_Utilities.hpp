/**
 * @file    OpenGL_Utilities.hpp
 * @author  Marvin Smith
 * @date    9/3/2015
*/
#ifndef __OPENGL_UTILITIES_HPP__
#define __OPENGL_UTILITIES_HPP__

// OpenGL
#include <GL/glew.h>

// C++ Libraries
#include <string>

/**
 * @brief Load Shaders and return GL Program.
*/
GLuint Load_Shaders( const std::string& vertex_file_path,
                     const std::string& fragment_file_path);




#endif
