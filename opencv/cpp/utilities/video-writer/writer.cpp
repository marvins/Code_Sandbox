
// OpenCV Libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

// C++ Libraries
#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

vector<string> Load_Image_Paths( const string& image_list_path )
{
    // Create output
    vector<string> output;

    // Open file
    ifstream fin;
    fin.open(image_list_path.c_str());
    
    // Read lines
    string line;
    getline( fin, line );

    while( fin.good() )
    {
        // Add the path
        output.push_back(line);

        // Read the next line
        getline( fin, line );

    }

    fin.close();
    
    // Sort the list
    std::sort( output.begin(),
               output.end() );

    // Return results
    return output;

}


int main( int argc, char* argv[] )
{
    // Set the output video name
    string video_output    = argv[1];

    // Get an image list
    string image_list_path = argv[2];

    // Load the list
    cout << "Loading image path list." << endl;
    vector<string> image_paths = Load_Image_Paths( image_list_path );
    
    // Load the first frame
    cout << "Loading first image" << endl;
    cv::Mat image = cv::imread( image_paths[0] );

    // Set the frame size
    cv::Size frame_size = image.size();


    // Create video writer
    cout << "Building video writer." << endl;
    cv::VideoWriter video_writer;
    bool is_color = true;
    double fps = 2;
    int four_cc = CV_FOURCC('M','P','4','2');//-1;


    // Open Video Writer
    video_writer.open( video_output.c_str(),
                       four_cc,
                       fps,
                       frame_size,
                       is_color );

    // Make sure it is open
    if( video_writer.isOpened() == false )
    {
        cerr << "Could not open the video writer." << endl;
        return -1;
    }

    // Iterate over images
    for( size_t i=1; i<image_paths.size(); i++ )
    {

        // Load image
        cout << "Loading image " << i << endl;
        image = cv::imread( image_paths[i] );

        // Write the image
        video_writer << image;

    }

    cout << "closing up" << endl;

    // Return success
    return 0;
}

