cmake_minimum_required( VERSION 3.0 FATAL_ERROR )

project( resource_manager_demo )

set(CMAKE_CXX_STANDARD 14)

add_executable( resource_manager_demo
                demo.cpp
                Resource_Manager.hpp
                SomeBlob.hpp )
