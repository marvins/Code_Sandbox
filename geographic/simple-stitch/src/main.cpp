/**
 * @file    main.cpp
 * @author  Marvin Smith
 * @date    10/26/2015
*/

// C++ Libraries
#include <iostream>


// GDAL Libraries
#include <gdal.h>

// Boost Libraries
#include <boost/filesystem.hpp>

// Project Libraries
#include "Frame_Set.hpp"
#include "Options.hpp"


/**
 * @brief Main Function
*/
int main( int argc, char* argv[] )
{
    // Variables
    int tile_id, frame_number;


    // Parse the Command-Line Options
    Options options(argc, argv);

    // Load Frame Sets
    std::map<int,Frame_Set> frame_sets;
    Load_Frame_Sets( options,
                     frame_sets );


    // List of tiles
    std::map<int,std::string> tile_list;

    // Iterate over frames
    auto frame_it = frame_sets.begin();
    for( ; frame_it != frame_sets.end(); frame_it++ )
    {
        // Get the frame number
        frame_number = frame_it->Get_Frame_Number();

        // Iterate over tiles
        tile_list = frame_sets.Get_Tile_List();

        auto tile_it = tile_list.begin();
        for( ; tile_it != tile_list.end(); tile_it++ )
        {
            // Get the Tile-ID
            tile_id = tile_it->first;

            // Get the path
            std::string image_path = tile_it->second;

            // Load the file


        }

    }


    // Exit Success
    return 0;
}


