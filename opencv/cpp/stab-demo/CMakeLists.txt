#    File:    CMakeLists.txt
#    Author:  Marvin Smith
#    Date:    10/30/2015
#
#    Purpose: Build the C++ OpenCV Video Writer
#

#  Configure C++11
set( CMAKE_CXX_FLAGS "-std=c++11 -Wall")

#---------------------------------#
#-     Change the Output Path    -#
#---------------------------------#
SET( EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin )
SET( LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib )


#--------------------#
#-    Find OpenCV   -#
#--------------------#
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDES} )


find_package( GDAL )

#-------------------#
#-    Find Boost   -#
#-------------------#
message("${Blue}Looking for Boost at ${BOOST_ROOT} ${ColourReset}")
set( Boost_USE_STATIC_LIBS ON )
find_package(Boost REQUIRED
        COMPONENTS
        system
        filesystem
        )
include_directories(${Boost_INCLUDE_DIRS})

#----------------------------#
#-    Define the Sources    -#
#----------------------------#
set( APP_SOURCES
        stabilization-demo.cpp
        Frame_Info.cpp
        Frame_Info.hpp
        Frame_Loader.cpp
        Frame_Loader.hpp
        Log_Utilities.cpp
        Log_Utilities.hpp
        Options.cpp
        Options.hpp
        String_Utilities.cpp
        String_Utilities.hpp
        )



#--------------------------------#
#-     Build the Executable     -#
#--------------------------------#
add_executable( opencv-video-stab
        ${APP_SOURCES}
        )

add_executable( opencv-optflow-demo
        opencv_optflow_demo.cpp )

add_executable( opencv_video_stab_temp
                opencv_video_stab.cpp
                cv/Deblurring.cpp
                cv/Deblurring.hpp
                cv/Fast_Marching.cpp
                cv/Fast_Marching.hpp
                cv/Fast_Marching_Inl.hpp
                cv/Frame_Source.cpp
                cv/Frame_Source.hpp
                cv/Global_Motion.cpp
                cv/Global_Motion.hpp
                cv/Inpainting.cpp
                cv/Inpainting.hpp
                cv/Log.cpp
                cv/Log.hpp
                cv/Motion_Core.hpp
                cv/Motion_Stabilizing.cpp
                cv/Motion_Stabilizing.hpp
                cv/Optical_Flow.cpp
                cv/Optical_Flow.hpp
                cv/Outlier_Rejection.cpp
                cv/Outlier_Rejection.hpp
                cv/Ring_Buffer.hpp
                cv/Stabilizer.cpp
                cv/Stabilizer.hpp
                cv/Wobble_Suppression.cpp
                cv/Wobble_Suppression.hpp )


#-------------------------------#
#-     Link the Executable     -#
#-------------------------------#
target_link_libraries( opencv-video-stab
                      ${OpenCV_LIBRARIES}
                      ${Boost_LIBRARIES}
                      ${GDAL_LIBRARY} )

target_link_libraries( opencv-optflow-demo
        ${OpenCV_LIBRARIES})

target_link_libraries( opencv_video_stab_temp
                       ${OpenCV_LIBRARIES} )
