/**
 * @file   BrowserPane.cpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#include "BrowserPane.hpp"

// Image-Browser Libraries
#include <src/core/FilesystemUtilities.hpp>
#include <src/core/Log_Utilities.hpp>
#include <src/core/Rect.hpp>
#include <src/core/Point.hpp>

#include <string>

using namespace std;


/********************************/
/*          Constructor         */
/********************************/
BrowserPane::BrowserPane( QWidget* parent ) 
  : QWidget(parent)
{

    // create main layout
    mainLayout = new QGridLayout;

    // create web gui
    webView = new QWebEngineView(this);
    mainLayout->addWidget( webView, 0, 0 );
    
    
    // create toolbar widget
    build_toolbar_widget();

    
    // set the page
	setUrl( string("file://localhost/") + current_working_directory() + string("/html/google_maps.html"));

    
    // set the layout
    setLayout( mainLayout );
    

    // listen for messages to reload the selected overlays
    connect( &message_service, 
             SIGNAL(reloadBrowserOverlaySignal()), 
             this, 
             SLOT(reloadBrowserOverlays()));
    
    // Listen for events in the view
    webView->installEventFilter(this);
}

/*********************************/
/*          Set the URL          */
/*********************************/
void BrowserPane::setUrl( const string& url ){
    
	string turl = url;
	for( size_t i=0; i<turl.size(); i++ ){
		if( turl[i] == '\\' ){
			turl[i] = '/';
		}
	}
    std::cout << "Setting QWebEngineView URL: " << turl << std::endl;
    webView->setUrl(QUrl(turl.c_str()));

}


/***********************************************/
/*          Build the Toolbar Widget           */
/***********************************************/
void BrowserPane::build_toolbar_widget()
{
    
    // create toolbar widget
    toolbarWidget = new QWidget;
    toolbarWidget->setSizePolicy( QSizePolicy::Maximum, 
                                  QSizePolicy::Minimum );
	
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


/************************************************/
/*          Reload the Browser Overlays         */
/************************************************/
void BrowserPane::reloadBrowserOverlays()
{
    
    /// Clear all overlays
    for( size_t i=0; i<settings.variables.size(); i++ ){
        string cc = settings.variables[i] + string(".setMap(null);");
        webView->page()->runJavaScript( cc.c_str() );
    }
    settings.variables.clear();

    /// Iterate over the overlay list, adding them to the map
    for( size_t i=0; i<settings.overlay_list.size(); i++ ){
     
        // create the javascript variable
        string varname;
        string command = settings.overlay_list[i].toGoogleMapsString( varname, i);
        webView->page()->runJavaScript( command.c_str());
    
        // add the variable to the list
        settings.variables.push_back(varname);
    }
    
}

/********************************************/
/*          Browser Event Filter            */
/********************************************/
bool BrowserPane::eventFilter(QObject *target, QEvent *event)
{
    // Check if we modified the webview
    if( target == webView )
    {
        // Check for Mouse
        if( event->type() == QEvent::MouseButtonPress )
        {
            sleep(1);
            webView->page()->runJavaScript("marker_lat", [&](const QVariant& result){
                Parse_Latitude( result.toString() ); });
            webView->page()->runJavaScript("marker_lon", [&](const QVariant& result){
                Parse_Longitude( result.toString() ); });
        }

        // Otherwise, print information
        else
        {
            std::cout << "Event Type: " << event->type() << std::endl;
        }
    }
    return false;
}

void BrowserPane::Parse_Latitude( const QString& lat_str )
{
    double lat;
    bool valid = false;
    try{
        lat = std::stod(lat_str.toStdString());
        valid = true;
    }

    catch(...){
        cerr << "Unable to parse the marker coordinates" << std::endl; 
    }

    
    if( valid ){
        std::cout << "Latitude: " << lat << std::endl;
    }
}

void BrowserPane::Parse_Longitude( const QString& lon_str )
{
    double lon;
    bool valid = false;
    try{
        lon = std::stod(lon_str.toStdString());
        valid = true;
    }

    catch(...){
        cerr << "Unable to parse the marker coordinates" << std::endl; 
    }

    
    if( valid ){
        std::cout << "Longitude: " << lon << std::endl;
    }
}


