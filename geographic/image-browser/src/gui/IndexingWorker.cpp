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

    // create a progress bar
    IndexingProgressDialog*  dialog = new IndexingProgressDialog;
    dialog->setWindowModality( Qt::ApplicationModal );
    dialog->show();

    // take the current base directory and get a list of images
    dialog->updateStatus("Loading File List");
    vector<string> results = file_list( settings.base_directory, true );
    dialog->updateValue(5);
    
    if( dialog->cancelIndexStatus() == true ){
        settings.database.clear();
        dialog->close();
        return;
    }

    // clear the current database
    dialog->updateStatus("Clearing Database");
    settings.database.clear();
    dialog->updateStatus("Loading Bounding Boxes");

    for( size_t i=0; i<results.size(); i++ ){
        
        
        if( dialog->cancelIndexStatus() == true ){
            settings.database.clear();
            dialog->close();
            return;
        }
        
        // filter the results to only gdal-compatible files
        if( GDALLoader::isValid( results[i] ) == true ){

            dialog->updateStatus(string("Loading: ")+results[i]);        
            settings.database.addItem( results[i] );
            dialog->updateValue(5 + (95*((double)i/(double)results.size())));
        }

    }
    dialog->close();
    
    

    // rebuild the image database
    emit message_service.reloadAssetTreeSignal();


}

