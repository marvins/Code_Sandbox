/**
 * @file    simple-server.cpp
 * @author  Marvin Smith
 * @date    2/15/2018
 */


// C++ Dependencies
#include <iostream>
#include <string>

// Project Dependencies
#include "Options.hpp"

// LibVNC Server
#include <rfb/rfb.h>

int main( int argc, char* argv[] )
{
    // Parse the system configuration
    std::cout << "Loading Configuration Information" << std::endl;
    Options options(argc, argv);

    // Build the VNC Server
    std::cout << "Creating the VNC Server Session." << std::endl;
    std::cout << " - Bits Per Sample  : " << options.Get_Bits_Per_Sample() << std::endl;
    std::cout << " - Samples Per Pixel: " << options.Get_Samples_Per_Pixel() << std::endl;
    std::cout << " - Bytes Per Pixel  : " << options.Get_Bytes_Per_Pixel() << std::endl;
    rfbScreenInfoPtr server = rfbGetScreen( &argc, argv, 
                                            options.Get_Screen_Width(),
                                            options.Get_Screen_Height(), 
                                            options.Get_Bits_Per_Sample(),
                                            options.Get_Samples_Per_Pixel(),
                                            options.Get_Bytes_Per_Pixel());
    
    // Build the Frame Buffer
    server->frameBuffer = new char[options.Get_Frame_Buffer_Size_Bytes()];

    rfbInitServer(server);

    rfbRunEventLoop(server, -1, FALSE);

    return 0;
}

