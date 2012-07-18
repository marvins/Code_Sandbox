#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "shader_processing.h"

/// @cond DOXY_IGNORE

typedef std::basic_string<GLchar> GLstring;

struct Shader {
  const char* filename;
  GLenum      type;
  GLstring    source;
};

void printColMajorMatArrayf(GLfloat mat[16])
{
  for ( int i = 0; i < 4; ++i )
  {
    std::cout << "( ";
    for ( int j = 0; j < 4; ++j )
      std::cout << std::setw(8) << std::setprecision(5) << mat[4*j+i] << ' ';
    std::cout << ")\n";
  }
  std::cout << std::endl;
}

bool readShaderSource(const char* filename, GLstring& file)
{
  // open file and check if it opened
  std::ifstream fin(filename);
  if ( !fin.good() )
    return false;
  
  // get length of file
  int len;
  fin.seekg(0, std::ios::end);
  len = fin.tellg();
  fin.seekg(0, std::ios::beg);

  // read entire file into single array
  char* buffer = new char[len+1];
  fin.read(buffer, len);
  fin.close();

  // null terminate
  buffer[len] = '\0';

  // copy and remove c-style string
  // (I don't like allocating memory in a function and relying
  //  the calling function to de-allocate it)
  file = buffer;

  delete [] buffer;
  return true;
}

/// @endconf DOXY_IGNORE

GLuint InitShader(const char* vShader, const char* fShader)
{
  Shader shaders[2];
  shaders[0].filename = vShader;
  shaders[0].type = GL_VERTEX_SHADER;
  shaders[1].filename = fShader;
  shaders[1].type = GL_FRAGMENT_SHADER;

  // create the program in the GPU
  GLuint program = glCreateProgram();

  for ( int i = 0; i < 2; ++i )
  {
    Shader& s = shaders[i];

    // read file contents onto ONE LINE, doesn't seem to work
    // if file passed to glShaderSource in multiple lines
    bool read = readShaderSource( s.filename, s.source );
   
    if ( !read )
    {
      std::cerr << "Failed to read " << s.filename << std::endl;
      exit( EXIT_FAILURE );
    }
    
    // First build the shader
    GLuint shader = glCreateShader( s.type );
    
    // Second load the source
    const GLchar* sourceCode = s.source.c_str();
    glShaderSource( shader, 1, const_cast<const GLchar**>(&sourceCode), NULL );
    
    // Third compile the shader
    glCompileShader( shader );

    // Make sure it compiled, other display error message
    GLint compiled;
    glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
    if ( !compiled )
    {
      std::cerr << s.filename << " failed to compile:" << std::endl;
      GLint logSize;
      glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logSize );
      char* logMsg = new char[logSize];
      glGetShaderInfoLog( shader, logSize, NULL, logMsg );
      std::cerr << logMsg << std::endl;
      delete [] logMsg;

      exit( EXIT_FAILURE );
    }

    glAttachShader( program, shader );
  }

  // link and error check
  glLinkProgram(program);

  GLint linked;
  glGetProgramiv( program, GL_LINK_STATUS, &linked );
  if ( !linked )
  {
    std::cerr << "Shader program failed to link" << std::endl;
    GLint logSize;
    glGetProgramiv( program, GL_INFO_LOG_LENGTH, &logSize);
    char* logMsg = new char[logSize];
    glGetProgramInfoLog( program, logSize, NULL, logMsg );
    std::cerr << logMsg << std::endl;
    delete [] logMsg;

    exit( EXIT_FAILURE );
  }

  // use program object
  glUseProgram(program);
  return program;
}

