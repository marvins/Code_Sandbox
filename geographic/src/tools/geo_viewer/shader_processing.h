#ifndef __SRC_TOOLS_GEOVIEWER_SHADER_PROCESSING_H__
#define __SRC_TOOLS_GEOVIEWER_SHADER_PROCESSING_H__

#include <GL/glut.h>

char* readShaderSource(const char* shaderFile);

GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

#endif
