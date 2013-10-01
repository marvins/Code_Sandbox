/**
 *  @file   AssetPane.cpp
 *  @author Marvin Smith
 *  @date   10/1/2013
 */
#include "AssetPane.hpp"

#include <QFileSystemModel>

#include <iostream>

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
    filesystemPath = new QLabel(settings.base_directory.c_str());
    filesystemLayout->addWidget( filesystemPath );

    // create filesystem headers
    filesystemHeaderItem = new QTreeWidgetItem();
    filesystemHeaderItem->setText(0, QString("File Name"));
    filesystemHeaderItem->setText(1, QString("Size (Bytes)"));
    filesystemHeaderItem->setText(2, QString("Path"));

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
    
    // create list
    assetTree = new QTreeWidget( assetWidget );
    assetLayout->addWidget( assetTree );

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

        item->setText(2,fileInfo.filePath());
        filesystemTree->addTopLevelItem(item);    
    }

}

void AssetPane::filesystemClicked(){

    QList<QTreeWidgetItem*> selectedItems = filesystemTree->selectedItems();

    for( int i=0; i<selectedItems.size(); i++ ){
        // set the base directory
        settings.base_directory += file_canonical( string("/") + string(selectedItems[i]->text(0).toLocal8Bit().constData()));
        
        filesystemPath->setText(settings.base_directory.c_str());

        // rebuild the directory tree
        build_directory_tree();
    }

}

