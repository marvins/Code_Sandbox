/**
 * @file    stitcher.cpp
 * @author  Marvin Smith
 * @date    5/11/2015
*/

// OpenCV Libraries
#include <opencv2/opencv_modules.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/features2d.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/stitching/detail/camera.hpp>
#include <opencv2/stitching/detail/matchers.hpp>
#include <opencv2/stitching/detail/motion_estimators.hpp>

// C++ Standard Libraries
#include <deque>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>


using namespace std;


/**
 * @class Options
 */
class Options{

    public:
        
        /**
         * @brief Constructor
         */
        Options( int argc, char* argv[] ){
            
            // Set Defaults
            m_feature_detector_name = "SURF";

            // Set the application name
            m_application_name = argv[0];

            // Iterate over arguments
            std::deque<std::string> args;
            for( int i=1; i<argc; i++ )
                args.push_back(argv[i]);


            // Process args
            std::string arg;
            while( args.size() > 0 ){

                // Get next argument
                arg = args.front();
                args.pop_front();

                // Check if help requested
                if( arg == "-h" ){
                    Usage();
                    std::exit(-1);
                }

                // Check if image requested
                else if( arg == "-i" ){
                    m_image_list.push_back(args.front());
                    args.pop_front();
                }

                // Check if output image requested
                else if( arg == "-o" ){
                    m_output_image = args.front();
                    args.pop_front();
                }

                // Check if feature detector specified
                else if( arg == "-f" ){
                    m_feature_detector_name = args.front();
                    args.pop_front();
                }

            }

            // Process the feature detector
            if( m_feature_detector_name == "SURF" ||
                m_feature_detector_name == "surf" )
            {
                m_feature_detector = cv::makePtr<cv::detail::SurfFeaturesFinder>();
            }
            else if( m_feature_detector_name == "ORB" ||
                     m_feature_detector_name == "orb" )
            {
                m_feature_detector = cv::makePtr<cv::detail::OrbFeaturesFinder>();
            }
            else{
                throw std::runtime_error("error: Invalid feature detector name.");
            }

        }

        /**
         * @brief Get the Input Image List.
         */
        std::vector<std::string> Get_Input_Image_List()const{
            return m_image_list;
        }


        /**
         * @brief Get the Feature Detector
         */
        cv::Ptr<cv::detail::FeaturesFinder> Get_Feature_Finder()const{
            return m_feature_detector;
        }

    
    private:

        /**
         * @brief Print Usage
         */
        void Usage(){

            std::cerr << "usage: " << m_application_name << " -o <output-image> -i <input-image> -i <input-image> ... <misc flags>" << std::endl;
            std::cerr << std::endl;
            std::cerr << "    misc flags: " << std::endl;
            std::cerr << "      -f <detector-name>  : Specify feature detector to use." << std::endl;
            std::cerr << std::endl;
        }
        
        /// Application Name
        std::string m_application_name;

        /// List of images
        std::vector<std::string> m_image_list;

        /// Output image
        std::string m_output_image;

        /// Feature Detector Finder
        cv::Ptr<cv::detail::FeaturesFinder> m_feature_detector;
        
        /// Feature Detector Name
        std::string m_feature_detector_name;

}; // End of Options Class

/**
 * @brief Main Function
 */
int main( int argc, char* argv[] )
{

    // Parse Command-Line
    Options options(argc, argv);

    
    // Load each image
    std::vector<std::string> image_path_list = options.Get_Input_Image_List();
    std::vector<cv::Mat> image_list;
    for( int i=0; i<image_path_list.size(); i++ ){
        image_list.push_back( cv::imread( image_path_list[i] ) );
    }
    std::vector<cv::Mat> image_list_gray(image_list.size());
    for( int i=0; i<image_path_list.size(); i++ ){
        cv::cvtColor( image_list[i], image_list_gray[i], CV_BGR2GRAY );
    }
    
    //  Define our feature detector
    cv::Ptr<cv::detail::FeaturesFinder> detector = options.Get_Feature_Finder();

    // Get features for each image
    std::vector<cv::detail::ImageFeatures> features(image_list.size());
    for( int i=0; i<image_list.size(); i++ )
    {
        // Find features
        (*detector)( image_list_gray[i], features[i] );   

    }


    // Do Pair-Wise Matching
    double match_conf = 0.3;
    std::vector<cv::detail::MatchesInfo> pairwise_matches;
    cv::detail::BestOf2NearestMatcher matcher( false, match_conf );
    matcher( features, pairwise_matches );

    
    // Select matches for panorama
    double conf_threash = 1.0;
    std::vector<int> indices = leaveBiggestComponent( features, pairwise_matches, conf_threash );
    
    
    // Iterate over indices to get the image names
    std::vector<cv::Mat>  img_subset;
    std::vector<string>   img_names_subset;
    std::vector<cv::Size> full_img_sizes_subset;
    
    for( int i=0; i<indices.size(); i++ )
    {
        // Push the names
        img_names_subset.push_back( image_path_list[indices[i]] );
        img_subset.push_back(image_list[indices[i]]);
        full_img_sizes_subset.push_back(image_list[indices[i]].size());
    }


    // Start Estimating Camera Parameters
    cv::detail::HomographyBasedEstimator estimator;
    std::vector<cv::detail::CameraParams> cameras;
    if( !estimator( features, pairwise_matches, cameras) )
    {
        std::cout << "Homography Estimation Failed." << std::endl;
        return -1;
    }


    // Exit Program
    return 0;
}


