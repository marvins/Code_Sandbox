/**
 * @file    simple-viewer.cpp
 * @author  Marvin Smith
 * @date    8/1/2017
*/

// Qt Libraries
#include <QApplication>

#include "Main_Window.hpp"

/**
 * @brief Main
 */
int main( int argc, char* argv[] )
{
    
    //   Create the Qt Application
    QApplication app(argc, argv);
    
    // Create Main Window
    Main_Window* main_window = new Main_Window();
    main_window->show();

    // Start Application
    return app.exec();
}
