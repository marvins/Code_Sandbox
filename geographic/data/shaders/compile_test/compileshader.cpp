#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#ifdef __APPLE__
#  include "OpenGL/OpenGL.h"
#  include "GLUT/glut.h"
#else
#  include <GL/glew.h>
#  include <GL/glut.h>
#endif

typedef std::basic_string<GLchar> GLstring;

GLstring ReadTextFile(const char* filename)
{
  GLstring file;

  // open file and check if it opened
  std::ifstream fin(filename);
  if ( !fin.good() )
    return GLstring("");
  
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
  return file;
}


int main(int argc, char *argv[])
{
   if ( argc <= 2 )
   {
      printf("Please enter the name of the vertex shader followed by the"
              "fragement shader.\n\n");  
      return -1;
   }

   GLstring vertexsource;
   GLstring fragmentsource;
   int IsLinked;
   int maxLength;
   char *vertexInfoLog;
   char *fragmentInfoLog;
   char *shaderProgramInfoLog;
   GLint IsCompiled_VS;
   GLint IsCompiled_FS;
   GLint shaderprogram;
   
   GLuint vertexshader;

   glutInit(&argc, argv);
   glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
   glutInitWindowSize( 50,50 );
   glutCreateWindow( "Test" ); 

#  ifndef __APPLE__
   glewInit();
#  endif

   /* Read our shaders into the appropriate buffers */
   /* You need to code your own version of ReadTextFile() */
   /* Also, the file extension that you give to your vertex and fragment shader doesn't matter. */
   /* Don't forget, OpenGL doesn't care about your files. */
   /* Let's assuming that ReadTextFile allocates a char array and fills it with text */
   vertexsource = ReadTextFile(argv[1]);
   fragmentsource = ReadTextFile(argv[2]);

   /* Create an empty vertex shader handle */
   vertexshader = glCreateShader(GL_VERTEX_SHADER);

   /* Send the vertex shader source code to GL */
   /* Note that the source code is NULL character terminated. */
   /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
   const GLchar* vsource = vertexsource.c_str();
   glShaderSource(vertexshader, 1, const_cast<const GLchar**>(&vsource), 0);

   /* Compile the vertex shader */
   glCompileShader(vertexshader);

   glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &IsCompiled_VS);
   if(IsCompiled_VS == GL_FALSE)
   {
      glGetShaderiv(vertexshader, GL_INFO_LOG_LENGTH, &maxLength);

      /* The maxLength includes the NULL character */
      vertexInfoLog = new char[maxLength];

      glGetShaderInfoLog(vertexshader, maxLength, &maxLength, vertexInfoLog);

      /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
      /* In this simple program, we'll just leave */
      std::cout << "Error: in " << argv[1] << std::endl;
      std::cout << vertexInfoLog << std::endl;
      
      delete [] vertexInfoLog;
      return -1;
   }

   /* Create an empty fragment shader handle */
   GLuint fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

   /* Send the fragment shader source code to GL */
   /* Note that the source code is NULL character terminated. */
   /* GL will automatically detect that therefore the length info can be 0 in this case (the last parameter) */
   const GLchar* fsource = fragmentsource.c_str();
   glShaderSource(fragmentshader, 1, const_cast<const GLchar**>(&fsource), 0);

   /* Compile the fragment shader */
   glCompileShader(fragmentshader);

   glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &IsCompiled_FS);
   if(IsCompiled_FS == GL_FALSE)
   {
      glGetShaderiv(fragmentshader, GL_INFO_LOG_LENGTH, &maxLength);

      /* The maxLength includes the NULL character */
      fragmentInfoLog = new char[maxLength];

      glGetShaderInfoLog(fragmentshader, maxLength, &maxLength, fragmentInfoLog);
      std::cout << "Error: in " << argv[2] << std::endl;
      std::cout << fragmentInfoLog << std::endl;

      /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
      /* In this simple program, we'll just leave */
      delete [] fragmentInfoLog;
      return -1;
   }

   /* If we reached this point it means the vertex and fragment shaders compiled and are syntax error free. */
   /* We must link them together to make a GL shader program */
   /* GL shader programs are monolithic. It is a single piece made of 1 vertex shader and 1 fragment shader. */
   /* Assign our program handle a "name" */
   shaderprogram = glCreateProgram();

   /* Attach our shaders to our program */
   glAttachShader(shaderprogram, vertexshader);
   glAttachShader(shaderprogram, fragmentshader);

   /* Link our program */
   /* At this stage, the vertex and fragment programs are inspected, optimized and a binary code is generated for the shader. */
   /* The binary code is uploaded to the GPU, if there is no error. */
   glLinkProgram(shaderprogram);

   /* Again, we must check and make sure that it linked. If it fails, it would mean either there is a mismatch between the vertex */
   /* and fragment shaders. It might be that you have surpassed your GPU's abilities. Perhaps too many ALU operations or */
   /* too many texel fetch instructions or too many interpolators or dynamic loops. */

   glGetProgramiv(shaderprogram, GL_LINK_STATUS, (int *)&IsLinked);
   if(IsLinked == GL_FALSE)
   {
      /* Noticed that glGetProgramiv is used to get the length for a shader program, not glGetShaderiv. */
      glGetProgramiv(shaderprogram, GL_INFO_LOG_LENGTH, &maxLength);

      /* The maxLength includes the NULL character */
      shaderProgramInfoLog = new char[maxLength];

      /* Notice that glGetProgramInfoLog, not glGetShaderInfoLog. */
      glGetProgramInfoLog(shaderprogram, maxLength, &maxLength, shaderProgramInfoLog);
      std::cout << "Error: in linking" << std::endl;
      std::cout << shaderProgramInfoLog << std::endl;

      /* Handle the error in an appropriate way such as displaying a message or writing to a log file. */
      /* In this simple program, we'll just leave */
      free(shaderProgramInfoLog);
      return -1;
   }

   /* In your rendering code, you just need to call glUseProgram, call the various glUniform** to update your uniforms */
   /* and then render. */
   /* Load the shader into the rendering pipeline */
   glUseProgram(shaderprogram);

   /* When the user shuts down your program, you should deallocate all your GL resources. */
   /* Unbind your shader. */
   glUseProgram(0);
   /* Let's detach */
   glDetachShader(shaderprogram, vertexshader);
   glDetachShader(shaderprogram, fragmentshader);
   /* Delete the shaders */
   glDeleteShader(vertexshader);
   glDeleteShader(fragmentshader);
   /* Delete the shader object */
   glDeleteProgram(shaderprogram);

   return 0;
}
