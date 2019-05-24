/**
 * @file    stabilization-checker.cpp
 * @author  Marvin Smith
 * @date    5/23/2019
 */
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>
#include <string>

#include "Interface.hpp"
#include "Log_Utilities.hpp"
#include "Options.hpp"


int main( int argc, char* argv[] )
{
    // Parse Configuration
    auto options = Options::Parse_Command_Line(argc, argv);

    // Create Interface
    auto gui = std::make_shared<Interface>(options);

    gui->Start();


    gui->Wait_Until_Completion();

    return 0;
}
