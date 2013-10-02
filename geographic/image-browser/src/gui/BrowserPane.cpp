/**
 * @file   BrowserPane.cpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#include "BrowserPane.hpp"

#include <src/core/FilesystemUtilities.hpp>
#include <src/core/Rect.hpp>
#include <src/core/Point.hpp>

#include <string>

using namespace std;

BrowserPane::BrowserPane( QWidget* parent ) : QWidget(parent){

    // create main layout
    mainLayout = new QGridLayout;

    // create web gui
    webView = new QWebView(this);
    mainLayout->addWidget( webView, 0, 0 );
    
    // create toolbar widget
    build_toolbar_widget();

    // set the page
    setUrl( string("file://localhost/") + current_working_directory() + string("/html/google_maps.html"));

    // set the layout
    setLayout( mainLayout );
    
    // listen for messages to reload the selected overlays
    connect( &message_service, SIGNAL(reloadBrowserOverlaySignal()), this, SLOT(reloadBrowserOverlays()));

}

void BrowserPane::setUrl( const string& url ){
    
    webView->setUrl(QUrl(url.c_str()));

}

void BrowserPane::build_toolbar_widget(){
    
    // create toolbar widget
    toolbarWidget = new QWidget;
    
    // create layout
    toolbarLayout = new QVBoxLayout;
    toolbarLayout->setAlignment( Qt::AlignTop );

    // create look at button
    toolbarLookAtButton = new QToolButton;
    toolbarLookAtButton->setIcon(QIcon("icons/lookat.png"));
    toolbarLookAtButton->setIconSize(QSize(30,30));
    toolbarLookAtButton->setToolTip("Look at a specific location");
    toolbarLayout->addWidget( toolbarLookAtButton );
    
    // create export button
    toolbarExportButton = new QToolButton;
    toolbarExportButton->setIcon(QIcon("icons/export.png"));
    toolbarExportButton->setIconSize(QSize(30,30));
    toolbarExportButton->setToolTip("Export selected imagery to KML");
    toolbarLayout->addWidget( toolbarExportButton );

    // set layout
    toolbarWidget->setLayout( toolbarLayout );

    // add to layout
    mainLayout->addWidget( toolbarWidget, 0, 1 );

}

void BrowserPane::reloadBrowserOverlays(){

    cout << "Reloading Overlays" << endl;
    cout << "Size: " << settings.overlay_list.size() << endl;
    for( size_t i=0; i<settings.overlay_list.size(); i++ ){
        cout << i << " : " << settings.overlay_list[i].shape << endl;
    }

}

