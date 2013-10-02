/**
 * @file   DataContainer.hpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#ifndef __SRC_CORE_DATACONTAINER_HPP__
#define __SRC_CORE_DATACONTAINER_HPP__

#include <string>
#include <vector>

#include <src/core/ImageDatabase.hpp>
#include <src/core/Overlay.hpp>

/**
 * @class DataContainer
 */
class DataContainer{

    public:

        /**
         * Default Constructor
        */
        DataContainer();

        /**
         * Load Container
        */
        void load( int argc, char* argv[] );

        /**
         * Load the container using a Unix configuration file
        */
        void load_unix_configuration();
        
        /**
         * Create Config File
        */
        void write( const std::string& configurationFilename = "" )const;

        /**
         * 
         */
        void read_file( const std::string& configurationFilename = "" );
        
        /**
         * test the key / value pair
        */
        void setItem( const std::string& key, const std::string& value );

        /// Base Directory
        std::string base_directory;

        /// Image Database
        ImageDatabase database;
        
        /// List of overlays
        std::vector<Overlay> overlay_list;

        /// Using Config File
        std::string config_filename;

        /// Indexing Progress Dialog Status
        std::string indexingProgressDialogStatus;
        
        /// Indexing Progress Dialog Value
        int  indexingProgressDialogValue;

        /// Indexing Progress Dialog Close Status
        bool indexingProgressDialogClose;

};


#endif
