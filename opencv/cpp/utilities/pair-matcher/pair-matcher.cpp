/**
 * @file    pair-matcher.cpp
 * @author  Marvin Smith
 * @date    2/15/2016
*/

// C++ Libraries
#include <iostream>

// Qt Libraries
#include <QApplication>

// Pair-Matcher Libraries
#include "Options.hpp"
#include "gui/MainWindow.hpp"



int main( int argc, char* argv[] )
{

    // Create Qt Application
    QApplication app(argc, argv);
    
    // Parse the Command-Line Options
    Options options( argc, argv);

    // Build the User-Interface
    MainWindow main_window( options );

    main_window.show();
    return app.exec();
}

