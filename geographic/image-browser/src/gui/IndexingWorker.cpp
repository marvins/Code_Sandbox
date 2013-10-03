/**
 * @file   IndexingWorker.cpp
 * @author Marvin Smith
 * @date   10/2/2013
*/
#include <src/gui/IndexingWorker.hpp>
#include <src/core/FilesystemUtilities.hpp>
#include <src/core/GDALLoader.hpp>
#include <src/gui/IndexingProgressDialog.hpp>

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/**
 * Start indexing the filesystem
*/
void IndexingWorker::startIndexing(){

    // create the progress bar
    emit message_service.showIndexingProgressDialogSlot();
    
    // take the current base directory and get a list of images
    vector<string> results = file_list( settings.base_directory, true );
    
    settings.indexingProgressDialogStatus = "Loading File List";
    settings.indexingProgressDialogValue  = 5;
    emit message_service.updateIndexingProgressDialogStatusSlot();
    
    // clear the current database
    settings.indexingProgressDialogStatus = "Clearing Database";
    emit message_service.updateIndexingProgressDialogStatusSlot();
    settings.database.clear();
    
    
    settings.indexingProgressDialogStatus = "Building Bounding Boxes";
    emit message_service.updateIndexingProgressDialogStatusSlot();

    for( size_t i=0; i<results.size(); i++ ){
        
        // filter the results to only gdal-compatible files
        if( GDALLoader::isValid( results[i] ) == true ){
            
            settings.indexingProgressDialogStatus = string("Loading: ")+results[i];        
            emit message_service.updateIndexingProgressDialogStatusSlot();
            
            settings.database.addItem( results[i] );
            
            settings.indexingProgressDialogValue = (5 + (95*((double)i/(double)results.size())));
            emit message_service.updateIndexingProgressDialogStatusSlot();
        
        }

    }

    
    // rebuild the image database
    emit message_service.reloadAssetTreeSlot();
    
    emit message_service.closeIndexingProgressDialogSlot();
}

