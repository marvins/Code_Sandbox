/**
 *  @file   AssetPane.cpp
 *  @author Marvin Smith
 *  @date   10/1/2013
 */
#include "AssetPane.hpp"

#include <src/core/FilesystemUtilities.hpp>
#include <src/core/Overlay.hpp>

#include <QFileSystemModel>

#include <iostream>
#include <vector>

using namespace std;

/**
 * Default Constructor
 */
AssetPane::AssetPane( QWidget* parent ) : QWidget( parent ){
    
    // create the main layout
    mainLayout = new QVBoxLayout;
    mainLayout->setAlignment( Qt::AlignTop );
    
    // create filesystem widget
    build_filesystem_widget( );

    // create asset widget
    build_asset_widget();

    // set the main layout
    setLayout( mainLayout );

    showOverlay = false;

}


void AssetPane::build_filesystem_widget(){
    
    // create main widget
    filesystemWidget = new QWidget(this);
    
    // create main layout
    filesystemLayout = new QVBoxLayout;

    // add label
    filesystemLabel = new QLabel("Base Image Directory");
    filesystemLabel->setFont(QFont(filesystemLabel->font().family(), 14));
    filesystemLayout->addWidget( filesystemLabel);
    
    // add path label
    filesystemPath = new QLineEdit;
    filesystemPath->setText(string(string("Base: ")+settings.base_directory).c_str());
    filesystemLayout->addWidget( filesystemPath );

    // create filesystem headers
    filesystemHeaderItem = new QTreeWidgetItem();
    filesystemHeaderItem->setText(0, QString("Directory Name"));

    // add tree
    filesystemTree = new QTreeWidget;
    filesystemTree->setHeaderItem(filesystemHeaderItem);
    filesystemLayout->addWidget( filesystemTree );
    connect( filesystemTree, SIGNAL(itemDoubleClicked( QTreeWidgetItem*, int)), this, SLOT(filesystemClicked()));
    
    // create directory tree
    build_directory_tree();
    
    // add layout to widget
    filesystemWidget->setLayout( filesystemLayout );

    // assign to layout
    mainLayout->addWidget( filesystemWidget );
    
    // connect the reloadAssetTree signal to the slot
    connect( &message_service, SIGNAL(reloadAssetTreeSignal()), this, SLOT(reloadAssetTree()));


}

void AssetPane::build_asset_widget(){

    // create main widget
    assetWidget = new QWidget(this);

    // create main layout
    assetLayout = new QVBoxLayout;

    // create main label
    assetLabel = new QLabel("Discovered Assets");
    assetLabel->setFont(QFont(assetLabel->font().family(), 14));
    assetLayout->addWidget( assetLabel );
    

    // create asset headers
    assetHeaderItem = new QTreeWidgetItem();
    assetHeaderItem->setText(0, QString("Select"));
    assetHeaderItem->setText(1, QString("File Name"));
    assetHeaderItem->setText(2, QString("Format"));
    assetHeaderItem->setText(3, QString("Full Path"));
    
    // create list
    assetTree = new QTreeWidget( assetWidget );
    assetTree->setHeaderItem( assetHeaderItem );

    assetTree->setColumnWidth(0,50);
    assetLayout->addWidget( assetTree );
    connect( assetTree, SIGNAL(itemClicked( QTreeWidgetItem*, int)), this, SLOT(assetSelected()));

    // create button bar
    build_asset_button_bar();

    // set layout
    assetWidget->setLayout( assetLayout );

    // assign to layout
    mainLayout->addWidget( assetWidget );


}

void AssetPane::build_directory_tree(){

    // create the directory
    QDir*  rootDir = new QDir(settings.base_directory.c_str());
    QFileInfoList  filesList = rootDir->entryInfoList();
    
    // clear the current list of items
    filesystemTree->clear();
    
    // add each directory to the list
    foreach( QFileInfo fileInfo, filesList ){

        // create a widget item
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0,fileInfo.fileName());
        
        // ignore if it is a file
        if(fileInfo.isFile())
        { 
            continue;
        }

        if(fileInfo.isDir())
        {
            item->setIcon(0,*(new QIcon("icons/folder.png")));
        } 

        filesystemTree->addTopLevelItem(item);    
    }

}

void AssetPane::filesystemClicked(){

    QList<QTreeWidgetItem*> selectedItems = filesystemTree->selectedItems();

    for( int i=0; i<selectedItems.size(); i++ ){
        
        // set the base directory
        settings.base_directory = file_canonical( settings.base_directory + string("/") + string(selectedItems[i]->text(0).toLocal8Bit().constData()));
        
        filesystemPath->setText(string( string("Base: ")+ settings.base_directory).c_str());

        // rebuild the directory tree
        build_directory_tree();
    }

}

void AssetPane::build_asset_button_bar(){

    // create button bar
    assetButtonBarWidget = new QWidget;

    // create layout
    assetButtonBarLayout = new QHBoxLayout;
    assetButtonBarLayout->setAlignment( Qt::AlignLeft );

    // create search button
    assetSearchButton = new QToolButton;
    assetSearchButton->setIcon(QIcon("icons/search.png"));
    assetSearchButton->setIconSize(QSize(40,40));
    assetSearchButton->setFixedWidth(42);
    assetSearchButton->setFixedHeight(42);
    assetSearchButton->setToolTip("Search for all imagery in folder");
    assetButtonBarLayout->addWidget( assetSearchButton );
    connect( assetSearchButton, SIGNAL(clicked()), this, SLOT(indexFilesystem()));

    // create show overlay button
    assetShowOverlayButton = new QToolButton;
    assetShowOverlayButton->setIcon(QIcon("icons/overlay.png"));
    assetShowOverlayButton->setIconSize(QSize(40,40));
    assetShowOverlayButton->setFixedWidth(42);
    assetShowOverlayButton->setFixedHeight(42);
    assetShowOverlayButton->setToolTip("Show Overlay on Google Maps");
    assetButtonBarLayout->addWidget( assetShowOverlayButton );
    connect( assetShowOverlayButton, SIGNAL(clicked()), this, SLOT(showOverlayClicked()));

    // set layout
    assetButtonBarWidget->setLayout( assetButtonBarLayout );

    // add to main layout
    assetLayout->addWidget( assetButtonBarWidget );


}

/**
 * Start the thread which will run indexing operations
*/
void AssetPane::indexFilesystem(){
  
    // Create the worker and the thread
    IndexingWorker* worker = new IndexingWorker;
    QThread* workerThread = new QThread(this);

    // connect the signals together
    connect(workerThread, SIGNAL(started()), worker, SLOT(startIndexing()));
    connect(workerThread, SIGNAL(finished()), worker, SLOT(deleteLater()));

    worker->moveToThread(workerThread);
    
    // start running
    workerThread->start();
    
        
}

void AssetPane::reloadAssetTree(){
    
    // clear the current list of items
    assetTree->clear();
    
    // iterate through each item in the image database
    for( size_t i=0; i<settings.database.size(); i++ ){

        // create a widget item
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setFlags( item->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
        
        item->setCheckState(0, Qt::Unchecked );
        item->setText( 1, settings.database[i].basename().c_str() );
        item->setText( 2, settings.database[i].formatShortString().c_str() );
        item->setText( 3, settings.database[i].canonical().c_str() );
        
        // set an icon
        item->setIcon( 1,*(new QIcon("icons/image.png")));

        assetTree->addTopLevelItem(item);    
    }

    /// resize column
    assetTree->resizeColumnToContents(1);
    assetTree->resizeColumnToContents(2);

}

void AssetPane::showOverlayClicked(){
    
    /// hold overlay button down
    if( showOverlay == false ){
        showOverlay = true;
        assetShowOverlayButton->setDown(true);

        // reload browser overlays
        assetSelected();
    }
    /// release hold overlay button
    else{
        showOverlay = false;
        assetShowOverlayButton->setDown(false);
    }
}

void AssetPane::assetSelected(){
    
    settings.overlay_list.clear();

    // if we are not showing the overlay, then do nothing
    if( showOverlay == false ) return;

    // create a list of everything that is activated
    for( int i=0; i<assetTree->topLevelItemCount(); i++ ){
        
        // make sure item has been checked
        if( assetTree->topLevelItem(i)->checkState(0) == Qt::Checked ){ 
            
            // find the bounding box of the top level item
            int idx = settings.database.findImage( assetTree->topLevelItem(i)->text(3).toLocal8Bit().constData() );
            if( idx >= 0 ){
            
                // bounding box
                Rect bbox = settings.database[idx].getBBox();
                
                // add to overlay list
                settings.overlay_list.push_back(Overlay(bbox,Qt::green));
            }
        }
    }
    
    // emit the signal
    emit message_service.reloadBrowserOverlaySlot();

}

