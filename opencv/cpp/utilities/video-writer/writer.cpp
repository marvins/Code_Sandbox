
// Project Libraries
#include "Options.hpp"


// OpenCV Libraries
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


// Boost Libraries
#include <boost/filesystem.hpp>


// C++ Libraries
#include <iostream>
#include <string>
#include <vector>

using namespace std;
namespace bf=boost::filesystem;

int main( int argc, char* argv[] )
{

    // Parse Command-Line Options
    Options options(argc, argv);


    const int video_sx = 1920;//3840;
    const int video_sy = 1080;//2160;
    const int video_ox = options.Get_Corner_Offset().x;
    const int video_oy = options.Get_Corner_Offset().y;


    // Fetch the Affine Transform
    cv::Mat affine_transform = options.Get_Affine_Transform();
    std::cout << "Affine" << std::endl;
    std::cout << affine_transform << std::endl;
    std::cout << std::endl;


    // Compute Corner For Overlays
    cv::Point overlay_path_corner( video_sx * 0.1, 
                                   video_sy * 0.95);

    // Retrieve the image list
    vector<string> image_paths = options.Get_Image_List();
    vector<string> base_image_paths;
    for( size_t i=0; i<image_paths.size(); i++ ){
        base_image_paths.push_back(bf::path(image_paths[i]).filename().native());
    }
    
    // Load the first frame
    cout << "Loading first image: " << image_paths[0] << endl;
    cv::Mat image = cv::imread( image_paths[0] );


    // Compute the Frame Size
    cout << "Computing Frame Size" << std::endl;
    cout << "Image Size: " << image.cols << " x " << image.rows << std::endl;
    cv::Rect roi_bbox = cv::Rect( 0, 0, image.cols, image.rows);
    roi_bbox = cv::Rect( video_ox, video_oy, 
                         video_sx, video_sy);


    cv::Size frame_size( roi_bbox.width, roi_bbox.height);


    // Set the frame size
    std::cout << "Frame size: " << frame_size << std::endl;
    std::cout << "ROI       : " << roi_bbox << std::endl;

    // Create video writer
    cout << "Building video writer." << endl;
    cv::VideoWriter video_writer;
    bool is_color = true;
    //int four_cc = CV_FOURCC('F','M','P','4');
    int four_cc = CV_FOURCC('X','2','6','4');// CV_FOURCC('M','P','4','2');//-1;
    


    // Open Video Writer
    cout << "FOUR_CC: " << four_cc << std::endl;
    video_writer.open( options.Get_Output_Pathname().c_str(),
                       four_cc,
                       options.Get_FPS(),
                       frame_size,
                       is_color );

    // Make sure it is open
    if( video_writer.isOpened() == false )
    {
        cerr << "Could not open the video writer." << endl;
        return -1;
    }

    // Iterate over images
    for( size_t i=0; i<image_paths.size(); i++ )
    {

        // Load image
        cout << "Loading image " << image_paths[i] << endl;
        image = cv::imread( image_paths[i] );
        cout << "Size: " << image.size() << std::endl;

        // Check image size
        if( image.rows < 1 || image.cols < 1 ){
            std::cerr << "Image is not valid.  Size: " << image.size() << ", Skipping." << std::endl;
            continue;
        }

        // Warp Image
        std::cout << "Warping Image" << std::endl;
        cv::warpAffine( image, image, affine_transform, image.size());


        // Crop to the bounding box
        std::cout << "Cropping Image" << std::endl;
        cv::Mat frame_image = cv::Mat(image, roi_bbox);

        // Overlay the text info if requested
        if( options.Get_Overlay_Path_Flag() == true )
        {
            // Write Text
            std::cout << "Writing Overlay" << std::endl;
            cv::putText( frame_image, 
                         base_image_paths[i], 
                         overlay_path_corner,
                         cv::FONT_HERSHEY_DUPLEX,
                         2,
                         cv::Scalar(0,255,0));
        }

        // Write the image
        std::cout << "Pushing to streamer" << std::endl;
        video_writer << frame_image;
        
        if( options.Use_GUI() ){
            cv::imshow("Video Output",frame_image);
            cv::waitKey(10);
        }
    }

    cout << "closing up" << endl;

    // Return success
    return 0;
}

