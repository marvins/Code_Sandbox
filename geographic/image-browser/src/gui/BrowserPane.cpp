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
    webView = new QWebView(this);
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
    std::cout << "Setting QWebView URL: " << turl << std::endl;
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
        webView->page()->mainFrame()->evaluateJavaScript( cc.c_str() );
    }
    settings.variables.clear();

    /// Iterate over the overlay list, adding them to the map
    for( size_t i=0; i<settings.overlay_list.size(); i++ ){
     
        // create the javascript variable
        string varname;
        string command = settings.overlay_list[i].toGoogleMapsString( varname, i);
        QVariant res = webView->page()->mainFrame()->evaluateJavaScript( command.c_str());
    
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
            std::string lat_res = webView->page()->mainFrame()->evaluateJavaScript("marker_lat").toString().toStdString();
            std::string lon_res = webView->page()->mainFrame()->evaluateJavaScript("marker_lon").toString().toStdString();
            double lat, lon;
            bool valid = false;
            try{
                lat = std::stod(lat_res); 
                lon = std::stod(lon_res); 
                valid = true;
            }
            catch(...){
                cerr << "Unable to parse the marker coordinates" << std::endl; 
            }

            if( valid ){
                std::cout << "Position: " << lat << ", " << lon << std::endl;
            }
        }

    }
    return false;
}

