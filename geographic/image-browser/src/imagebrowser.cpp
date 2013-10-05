/**
 *  @file   imagebrowser.cpp
 *  @author Marvin Smith
 *  @date   10/1/2013
 */
#include <QtGui>
#include <QApplication>

#include <src/gui/MainWindow.hpp>

#include <iostream>
#include <string>

#include <src/core/DataContainer.hpp>
#include <src/core/MessagingService.hpp>

DataContainer    settings;
MessagingService message_service;

using namespace std;

int main( int argc, char* argv[] ){

	///   Create the Qt Application
    QApplication app(argc, argv);
    
    // parse command line options
    settings.load( argc, argv );        
        
	///   Create main window
	MainWindow*  mainWindow = new MainWindow( );

	/// Display the main window
	mainWindow->show();
	
	/// Execute the application
	return app.exec();
}

