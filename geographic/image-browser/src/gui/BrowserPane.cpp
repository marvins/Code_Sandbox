/**
 * @file   BrowserPane.cpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#include "BrowserPane.hpp"

#include <src/core/FilesystemUtilities.hpp>

#include <string>

using namespace std;

BrowserPane::BrowserPane( QWidget* parent ) : QWidget(parent){

    // create main layout
    mainLayout = new QGridLayout;

    // create web gui
    webView = new QWebView(this);
    mainLayout->addWidget( webView, 0, 0 );
    
    // set the page
    setUrl( string("file://localhost/") + current_working_directory() + string("/html/google_maps.html"));

    // set the layout
    setLayout( mainLayout );

}

void BrowserPane::setUrl( const string& url ){
    
    webView->setUrl(QUrl(url.c_str()));

}
