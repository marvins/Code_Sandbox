/**
 * @file    geo-data-manager.cpp
 * @author  Marvin Smith
 * @date    3/2/2018
 */

// C++ Libraries
#include <iostream>


// Qt Libraries
#include <QApplication>


// Project Libraries
#include "core/Geo_Options.hpp"
#include "gui/Main_Window.hpp"


/**
 * @brief Main Function
 */
int main( int argc, char* argv[] )
{

    // Parse the Command-Line Options
    Geo_Options options(argc, argv);

    // Qt App Setup
    QApplication app(argc, argv);

    // Main Window
    auto main_window = new Main_Window(options);

    main_window->show();

    return app.exec();
}
