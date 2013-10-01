/**
 *  @file    DataContainer.cpp
 *  @author  Marvin Smith
 *  @date    10/1/2013
 */
#include "DataContainer.hpp"

#include <src/core/FilesystemUtilities.hpp>

DataContainer::DataContainer(){

    // base directory
    base_directory = current_working_directory();
}

