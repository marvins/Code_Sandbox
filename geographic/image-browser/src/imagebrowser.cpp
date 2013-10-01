/**
 *  @file   imagebrowser.cpp
 *  @author Marvin Smith
 *  @date   10/1/2013
 */
#include <QtGui>
#include <QApplication>

#include <src/gui/MainWindow.hpp>

#include <string>

#include <src/core/DataContainer.hpp>
DataContainer settings;

using namespace std;

int main( int argc, char* argv[] ){

    // parse command line options
        
        
	///   Create the Qt Application
    QApplication app(argc, argv);
        
	///   Create main window
	MainWindow*  mainWindow = new MainWindow( );

	/// Display the main window
	mainWindow->show();
	
	/// Execute the application
	return app.exec();
}

