

// C++ Libraries
#include <iostream>
#include <unistd.h>

// Project Libraries
#include "Window_Utilities.hpp"


using namespace std;

int main( int argc, char* argv[] )
{
    // Window
    Render_Window window(Render_Config("../data/Image-Utility.xml"));

    // Initialize the Window
    window.Initialize();

    sleep(5);

    // Finalize the Window
    window.Finalize();

    // Exit
    return 0;
}

