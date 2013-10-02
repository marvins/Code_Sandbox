/**
 *  @file   AssetPane.cpp
 *  @author Marvin Smith
 *  @date   10/1/2013
 */
#include "AssetPane.hpp"

#include <src/core/FilesystemUtilities.hpp>

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


}


void AssetPane::build_filesystem_widget(){
    
    // create main widget
    filesystemWidget = new QWidget(this);
    
    // create main layout
    filesystemLayout = new QVBoxLayout;

    // add label
    filesystemLabel = new QLabel("Base Directory");
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


}

void AssetPane::build_asset_widget(){

    // create main widget
    assetWidget = new QWidget(this);

    // create main layout
    assetLayout = new QVBoxLayout;

    // create main label
    assetLabel = new QLabel("Discovered Assets");
    assetLayout->addWidget( assetLabel );
    

    // create asset headers
    assetHeaderItem = new QTreeWidgetItem();
    assetHeaderItem->setText(0, QString("Select"));
    assetHeaderItem->setText(1, QString("File Name"));
    assetHeaderItem->setText(2, QString("Format"));
    assetHeaderItem->setText(3, QString("Description"));
    
    // create list
    assetTree = new QTreeWidget( assetWidget );
    assetTree->setHeaderItem( assetHeaderItem );

    assetTree->setColumnWidth(0,50);
    assetLayout->addWidget( assetTree );
    
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

    // set layout
    assetButtonBarWidget->setLayout( assetButtonBarLayout );

    // add to main layout
    assetLayout->addWidget( assetButtonBarWidget );


}

void AssetPane::indexFilesystem(){
    
    // take the current base directory and get a list of images
    vector<string> results = file_list( settings.base_directory, true );
    
    // filter the results to only gdal-compatible files
    results = GDALLoader::filter( results );

    // clear the current database
    settings.database.clear();
    
    // add the results to the database
    for( size_t i=0; i<results.size(); i++ ){
        settings.database.addItem( results[i] );
    }
    
    // rebuild the image database
    build_asset_tree();
    
}

void AssetPane::build_asset_tree(){
    
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
        item->setText( 3, settings.database[i].formatLongString().c_str() );
        
        // set an icon
        item->setIcon( 1,*(new QIcon("icons/image.png")));

        assetTree->addTopLevelItem(item);    
    }

    /// resize column
    assetTree->resizeColumnToContents(1);
    assetTree->resizeColumnToContents(2);

}
