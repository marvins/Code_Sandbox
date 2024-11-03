/**
 * @file    Options.cpp
 * @author  Marvin Smith
 * @date    2/15/2018
 */
#include "Options.hpp"

// C++ Libraries
#include <deque>
#include <iostream>
#include <stdexcept>

// Internal Libs
#include "../common/Parser.hpp"

/********************************/
/*          Constructor         */
/********************************/
Options::Options( int argc, char* argv[] )
{
    // Misc args
    std::string arg;
    bool found;
    
    // Set the program name
    m_program_name = argv[0];

    // Config Path
    if( argc <= 1 ){
        std::cerr << "error: missing config path." << std::endl;
        Usage();
        std::exit(1);
    }
    std::string config_path = argv[1];

    //  Parse the config path
    PSR::Parser parser(config_path);

    // Get the screen sizes
    m_screen_width  = parser.getItem_int("SCREEN_WIDTH",  found );
    if( !found ){
        std::cerr << "error: Unable to find SCREEN_WIDTH parameter in config." << std::endl;
        std::exit(1);
    }

    m_screen_height = parser.getItem_int("SCREEN_HEIGHT", found );
    if( !found ){
        std::cerr << "error: Unable to find SCREEN_HEIGHT parameter in config." << std::endl;
        std::exit(1);
    }

    // Get Pixel Information
    m_bits_per_sample = parser.getItem_int("BITS_PER_SAMPLE", found );
    if( !found ){
        std::cerr << "error: Unable to find BITS_PER_SAMPLE parameter in config." << std::endl;
        std::exit(1);
    }
    m_samples_per_pixel = parser.getItem_int("SAMPLES_PER_PIXEL", found );
    if( !found ){
        std::cerr << "error: Unable to find SAMPLES_PER_PIXEL parameter in config." << std::endl;
        std::exit(1);
    }
    m_bytes_per_pixel = parser.getItem_int("BYTES_PER_PIXEL", found );
    if( !found ){
        std::cerr << "error: Unable to find BYTES_PER_PIXEL parameter in config." << std::endl;
        std::exit(1);
    }

    /// Network Information
    m_network_port = parser.getItem_int("NETWORK_PORT", found );
    if( !found ) {
        std::cerr << "error: Unable to find NETWORK_PORT parameter in config." << std::endl;
        std::exit(1);
    }
}


/**********************************************************/
/*          Print Application Usage Instructions          */
/**********************************************************/
void Options::Usage()
{
    std::cerr << "usage: " << m_program_name << " <config-path>" << std::endl;
}


/************************************************/
/*          Build the OpenCV PixelType          */
/************************************************/
int Options::Get_OpenCV_PType() const
{
    int32_t output = -1;

    switch(m_bits_per_sample)
    {
        case 8:

            switch(m_samples_per_pixel)
            {
                case 3:
                    switch(m_bytes_per_pixel)
                    {
                        case 4:
                            output = CV_8UC4;
                            break;
                    }
                    break;
            }
            break;

    }

    if( output < 0 ){
        throw std::runtime_error("Unsupported combination.");
    }
    return output;
}
