#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include <string>

#include <opencv2/core/core.hpp>

#include <GeoImage.h>

#include "Logger.hpp"
#include "Parser.hpp"
#include "../math/Quaternion.hpp"

/**
 * @class Options
 *
 * Configuration option class. Contains all of the major configuration values. 
*/
class Options{


    public:
        
        /**
         * Default Constructor 
         * 
         * @param[in] argc Number of arguments
         * @param[in] argv Arguement list
        */
        Options( const int& argc, char ** argv );
        
        /**
         * Print all configuration options.
        */
        void print();
        

        /**
         * Get run type
         * 
         * @return Run Type
        */
        std::string get_run_type()const;

        
        /**
         * Get the focal length of the imager
         *
         * @return focal length
        */
        double get_focal_length()const;
        
        /** 
         * Set the focal length
         * 
         * @param[in] fl focal length
        */
        void   set_focal_length( double const& fl );

        /**
         * Get the rectify image type
         *
         * @return OpenCV Image Type
         *  
         * Possible values
         * - CV_8UC1
         * - CV_8UC3
        */
        int      get_rectify_image_type()const;

        
        /**
         * Do Z Buffering
         * 
         * @brief This relates to the Perspective image module.
         *
         * @return True if you want to z buffering
        */
        bool doZBuffering()const;
        
        /**
         * Do Perspective to Parallel Projection Transformation. 
         * 
         * @return True if you want to do Perspective to Parallel Transformation
        */
        bool doPerspective2Parallel()const;

        /** 
         * If you are building the image, 
         * this will return the desired image size
         *
         * @return image size
        */
        cv::Size get_build_image_size()const;
        
        /** 
         * If you are building the image, 
         * this will return the desired image type
         * @return Image Type
        */
        int      get_build_image_type()const;
        
        //position of the original camera
        cv::Mat Position_i;

        //position of the rectified camera
        cv::Mat Position_f;
        
        ///rotation matrix for the quaternion
        cv::Mat RotationM;
        
        ///rotation quaternion
        Quaternion RotationQ;
        
        cv::Mat image; /*< Image being used */
        cv::Mat dem;   /*< DEM Image being used */
        
        Logger logger; /*< Logger */
        
        std::string rectify_output_filename;    /*< Name of Fully Rectified Image */

        cv::Mat get_build_img2cam()const;
        cv::Mat get_output_img2cam( cv::Size const& sz )const;
        cv::Mat get_build_cam2img()const;
        cv::Mat get_output_cam2img( cv::Size const& sz )const;
        
        cv::Mat get_output_cam2img( cv::Size const& sz, double const& ref )const;
        cv::Mat get_output_img2cam( cv::Size const& sz, double const& ref )const;
        
        

    private:
        
        /** 
         * Load the contents of the configuration file. 
         *
         * argc Number of command-line arguments
         * argv List of command-line arguments
        */
        void load_configuration( const int& argc, char ** argv );
        
        void load_camera_params_file();
        void load_camera_params_geo();
        
        std::string run_type;          /*<  Run Type Variable */
        
        bool perspective2parallel;     /*< Perspective 2 Parallel Flag for RECTIFY Module */
        bool zbufferEnabled;           /*< Z Buffer Flag for BUILD Module */

        std::string image_filename;    /*< Name of Image */
        std::string config_filename;   /*< Name of Configuration File */
        
        double focal_length;    /*< Camera Focal Length */

        cv::Size build_image_size;      /*< Size of test build image */
        std::string build_image_type;   /*< Build Image Type */
        std::string rectify_image_type; /*< Rectify Image Type */

        bool   rectify_is_geo;   /*< Tells us if the input image is a geographic image */
        string dem_mode;         /*< DEM Mode */

        PSR::Parser parser;
};



#endif
