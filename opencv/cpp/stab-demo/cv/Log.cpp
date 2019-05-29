/**
 * @file    Log.cpp
 */
#include "Log.hpp"

// C++ Libraries
#include <cstdio>
#include <cstdarg>


void LogToStdout::print(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    fflush(stdout);
    va_end(args);
}

