/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    9/29/2017
*/

// Qt Libraries
#include <QApplication>

// Project Libraries
#include "Main_Window.hpp"


/**
 * @brief Main Method
 * @param argc
 * @param argv
 * @return
 */
int main( int argc, char* argv[] )
{
    // Create the Qt Application
    QApplication app(argc, argv);

    // Create the Main Widget
    Main_Window* main_window = new Main_Window();

    // Show the GUI
    main_window->show();


    // Start the Application and Release Control
    return app.exec();
}