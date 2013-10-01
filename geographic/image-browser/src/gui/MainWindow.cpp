/**
 * @file   MainWindow.cpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#include "MainWindow.hpp"

MainWindow::MainWindow( ){

    // create splitter
    splitter = new QSplitter(this);
    
    // create asset pane
    assetPane = new AssetPane(splitter);
    splitter->addWidget( assetPane );

    // create web gui
    webView = new QWebView(splitter);
    splitter->addWidget( webView );

    // set central widget
    setCentralWidget( splitter );

}

