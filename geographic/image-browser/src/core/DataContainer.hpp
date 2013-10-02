/**
 * @file   DataContainer.hpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#ifndef __SRC_CORE_DATACONTAINER_HPP__
#define __SRC_CORE_DATACONTAINER_HPP__

#include <string>

#include <src/core/ImageDatabase.hpp>

/**
 * @class DataContainer
 */
class DataContainer{

    public:
    
        DataContainer();

        /// Base Directory
        std::string base_directory;

        /// Image Database
        ImageDatabase database;


};


#endif
