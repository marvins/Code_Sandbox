/**
 * @file    OpenCL_Utilities.hpp
 * @author  Marvin Smith
 * @date    2/3/2018
 */
#ifndef LIB_CONVERT_02_OPENCL_UTILITIES_HPP
#define LIB_CONVERT_02_OPENCL_UTILITIES_HPP

// OpenCL Libraries
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

// C++ Libraries
#include <cstdio>
#include <cstdlib>


#define LOG_OCL_ERROR(x, STRING )  if(x!=CL_SUCCESS) {printf( "\nLine No: %d ", __LINE__ ); printf(STRING); printf("\n    Error= %d\n",x); exit(-1); }

#define LOG_OCL_COMPILER_ERROR(PROGRAM, DEVICE)                                          \
        {                                                                                \
            cl_int logStatus;                                                            \
            char * buildLog = NULL;                                                      \
            size_t buildLogSize = 0;                                                     \
            logStatus = clGetProgramBuildInfo(PROGRAM,                                   \
                                              DEVICE,                                    \
                                              CL_PROGRAM_BUILD_LOG,                      \
                                              buildLogSize,                              \
                                              buildLog,                                  \
                                              &buildLogSize);                            \
            if(logStatus != CL_SUCCESS)                                                  \
            {                                                                            \
                printf( "Error # %d logStatus", logStatus );                             \
                printf( ":: clGetProgramBuildInfo<CL_PROGRAM_BUILD_LOG> failed.");       \
                exit(1);                                                                 \
            }                                                                            \
                                                                                         \
            buildLog = (char*)malloc(buildLogSize);                                      \
            if(buildLog == NULL)                                                         \
            {                                                                            \
                printf("Failed to allocate host memory. (buildLog)\n");                  \
                exit(1);                                                                 \
            }                                                                            \
            memset(buildLog, 0, buildLogSize);                                           \
                                                                                         \
            logStatus = clGetProgramBuildInfo(PROGRAM,                                   \
                                              DEVICE,                                    \
                                              CL_PROGRAM_BUILD_LOG,                      \
                                              buildLogSize,                              \
                                              buildLog,                                  \
                                              NULL);                                     \
            if(logStatus != CL_SUCCESS)                                                  \
            {                                                                            \
                printf( "Error # %d logStatus ", logStatus);                             \
                printf( ":: clGetProgramBuildInfo<CL_PROGRAM_BUILD_LOG> failed.");       \
                exit(1);                                                                 \
            }                                                                            \
                                                                                         \
            printf(" \n\t\t\tBUILD LOG\n");                                              \
            printf(" ************************************************\n");               \
            printf("%s",buildLog);                                                            \
            printf(" ************************************************\n");               \
            free(buildLog);                                                              \
            exit(1);                                                              \
        }

#endif