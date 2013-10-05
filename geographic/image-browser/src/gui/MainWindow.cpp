/**
 * @file   MainWindow.cpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#include "MainWindow.hpp"

#include <src/core/FilesystemUtilities.hpp>

#include <iostream>

using namespace std;

MainWindow::MainWindow( ){

    // create splitter
    splitter = new QSplitter(this);
    
    // create asset pane
    assetPane = new AssetPane(splitter);
    splitter->addWidget( assetPane );

    // create browser pane
    browserPane = new BrowserPane(splitter);
    splitter->addWidget( browserPane );

    // set central widget
    setCentralWidget( splitter );
    
    // build menus
    build_menu();
    
    indexingProgressDialog = NULL;

    // build connections
    connect( &message_service, SIGNAL(showIndexingProgressDialogSignal()),  this, SLOT(showIndexingProgressDialog()));
    connect( &message_service, SIGNAL(closeIndexingProgressDialogSignal()), this, SLOT(closeIndexingProgressDialog()));
    connect( &message_service, SIGNAL(updateIndexingProgressDialogStatusSignal()), this, SLOT(updateIndexingProgressDialog()));

}

void MainWindow::build_menu(){

    // create file menu
    fileMenu = menuBar()->addMenu(tr("&File"));
    
    // add quit action for toolbar
    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcuts(QKeySequence::Quit);
    quitAction->setStatusTip("Quit Program");
    connect( quitAction, SIGNAL(triggered()), this, SLOT(close()));
    fileMenu->addAction( quitAction );

    // create options menu
    optionsMenu = menuBar()->addMenu(tr("&Options"));
    
    // add preference pane
    prefAction = new QAction(tr("&Preferences"), this);
    prefAction->setStatusTip("Program Options");
    connect( prefAction, SIGNAL(triggered()), this, SLOT( loadPreferencePane()));
    optionsMenu->addAction(prefAction);

}

void MainWindow::loadPreferencePane(){

    // create a preference pane
    PreferenceDialog dialog(this);

    dialog.exec();

}

void MainWindow::showIndexingProgressDialog(){
    
    cout << "Showing Dialog" << endl;
    indexingProgressDialog = new IndexingProgressDialog;
    indexingProgressDialog->show();
}

void MainWindow::updateIndexingProgressDialog(){

    cout << "Updating" << endl;
    if( indexingProgressDialog != NULL ){

        indexingProgressDialog->updateValue(  settings.indexingProgressDialogValue );
        indexingProgressDialog->updateStatus( settings.indexingProgressDialogStatus );
    }
    cout << "End of updating" << endl;
}   

void MainWindow::closeIndexingProgressDialog(){

    cout << "Closing Dialog" << endl;
    if( indexingProgressDialog != NULL ){
        indexingProgressDialog->hide();
        indexingProgressDialog->close();
        indexingProgressDialog->deleteLater();
        indexingProgressDialog = NULL;
    }
    else{
        cout << "Already Closed" << endl;
    }
}

