/**
 * @file    Options.cpp
 * @author  Marvin Smith
 * @date    10/30/2015
 */
#include "Options.hpp"

// C++ Libraries
#include <algorithm>
#include <deque>
#include <fstream>
#include <iostream>


// Boost Libraries
#include <boost/filesystem.hpp>


// OpenCV Libraries
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


namespace bf=boost::filesystem;

/*****************************/
/*          Options          */
/*****************************/
Options::Options( int argc, char* argv[] )
 : m_class_name("Options"),
   m_overlay_path(false),
   m_image_scale(1),
   m_image_rotation(0),
   m_image_center_x(0.5),
   m_image_center_y(0.5),
   m_fps(2)
{
    // Parse the Command-Line
    Parse_Command_Line( argc, argv );

    // Validate
    Validate();

}


/******************************************/
/*          Parse the Command-Line        */
/******************************************/
void Options::Parse_Command_Line( int argc, 
                                  char* argv[] )
{

    // Set the program name
    m_program_name = argv[0];
    

    // Set the defaults
    m_corner_offset = cv::Point(0,0);

    // Create list of args
    std::deque<std::string> args;
    for( int i=1; i<argc; i++ ){
        args.push_back(argv[i]);
    }


    // Parse all arguments
    std::string arg;
    while( args.size() > 0 )
    {
        // Fetch the next argument
        arg = args.front();
        args.pop_front();

        // Check if help requested
        if( arg == "-h" || arg == "--help" )
        {
            Usage();
            std::exit(0);
        }
        
        // Image List File
        else if( arg == "-i" )
        {
            // Get the Argument
            std::string image_list_pathname = args.front();
            args.pop_front();

            // Load the image list
            std::vector<std::string> newlist = Load_Image_Path_File(image_list_pathname);
            for( size_t ii=0; ii<newlist.size(); ii++)
                m_image_list.push_back(newlist[ii]);
        }

        // Video Output Pathname
        else if( arg == "-v" )
        {
            // Set the path
            m_output_pathname = args.front();
            args.pop_front();
        }

        // Scale
        else if( arg == "-s" ){
            m_image_scale = std::stod(args.front());
            args.pop_front();
        }

        // Rotation
        else if( arg == "-r" ){
            m_image_rotation = std::stod(args.front());
            args.pop_front();
        }

        // Center X
        else if( arg == "-cx" ){
            m_image_center_x = std::stod(args.front());
            args.pop_front();
        }
        else if( arg == "-cy" ){
            m_image_center_y = std::stod(args.front());
            args.pop_front();
        }
        
        // Overlay Path Flag
        else if( arg == "--overlay-path" ){
            m_overlay_path = true;
        }

        // Check if Offset Requested
        else if( arg == "-ox" ){
            m_corner_offset.x = std::stoi(args.front());
            args.pop_front();
        }
        else if( arg == "-oy" ){
            m_corner_offset.y = std::stoi(args.front());
            args.pop_front();
        }

        // Frames per second
        else if( arg == "-fps" ){
            m_fps = std::stoi(args.front());
            args.pop_front();
        }

        // Number of threads
        else if( arg == "-nt" ){
            m_num_threads = std::stoi(args.front());
            args.pop_front();
        }

        // Otherwise, error
        else
        {
            std::cerr << "error: Unknown argument (" << arg << ")" << std::endl;
            Usage();
            std::exit(-1);
        }

    }

    // Get Image Size
    cv::Size image_size = cv::imread( m_image_list[0] ).size();
    
    // Compute Affine Transform
    cv::Point center(image_size.width  * m_image_center_x,
                     image_size.height * m_image_center_y);

    m_affine_transform = cv::getRotationMatrix2D( center, 
                                                  m_image_rotation,
                                                  m_image_scale );

}


/********************************/
/*          Validate            */
/********************************/
void Options::Validate()
{

    // Make sure image paths exists
    for( int i=0; i<(int)m_image_list.size(); i++ )
    {
        // Set image paths
        if( bf::exists(bf::path(m_image_list[i])) == false )
        {
            std::cerr << "warning: " << m_image_list[i] << " does not exist. Removing from image list." << std::endl;
            m_image_list.erase(m_image_list.begin() + i);
            i--;
        }
    }
}


/**********************************************/
/*          Print Usage Instructions          */
/**********************************************/
void Options::Usage()const
{
    // Print usage
    std::cerr << "usage: " << m_program_name << " [optional-flags] -v <output-video-path> -i <image-list-file>" << std::endl;
    std::cerr << std::endl;
    std::cerr << "optional rendering flags" << std::endl;
    std::cerr << std::endl;
    std::cerr << "  --overlay-path : Overlay the image path over the video. Default: " << std::boolalpha << m_overlay_path << std::endl;
    std::cerr << std::endl;
    std::cerr << "  -ox <int>      : Offset from top-left corner to place image." << std::endl;
    std::cerr << "  -oy <int>      : Offset from top-left corner to place image." << std::endl;
    std::cerr << std::endl;
    std::cerr << "  -s <double>    : Modify the image scale. Default: " << m_image_scale << std::endl;
    std::cerr << "  -r <double>    : Modify the rotation angle (Degrees). Default: " << m_image_rotation << std::endl;
    std::cerr << "  -cx <double>   : Modify the center x position (0-1). Default: " << m_image_center_x << std::endl;
    std::cerr << "  -cy <double>   : Modify the center y position (0-1). Default: " << m_image_center_y << std::endl;
    std::cerr << std::endl;
    std::cerr << "  -fps <int>     : Set the Frames Per Second. Default: " << m_fps << std::endl;
    std::cerr << std::endl;

    std::cerr << "optional performance flags" << std::endl;
    std::cerr << std::endl;
    std::cerr << "  -nt <int>      : Use multiple threads to load data. Default: 1" << std::endl;
    std::cerr << std::endl;
}


/****************************************/
/*          Load Image Paths            */
/****************************************/
std::vector<std::string> Options::Load_Image_Path_File( const std::string& image_list_path )
{
    // Create output
    std::vector<std::string> output;

    // Open file
    std::ifstream fin;
    fin.open(image_list_path.c_str());
    
    // Read lines
    std::string line;
    std::getline( fin, line );

    while( fin.good() )
    {
        // Add the path
        output.push_back(line);

        // Read the next line
        getline( fin, line );

    }

    fin.close();
    
    // Sort the list
    //std::sort( output.begin(),
    //           output.end() );

    // Return results
    return output;

}


