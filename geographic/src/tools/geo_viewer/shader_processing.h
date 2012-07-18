#ifndef __SHADER_PROCESSING_H__
#define __SHADER_PROCESSING_H__

#include <cstdlib>
#include <iostream>
#include <stdio.h>

#include "GL.h"

using namespace std;

char* readShaderSource(const char* shaderFile);

GLuint InitShader(const char* vShaderFile, const char* fShaderFile);

#endif
