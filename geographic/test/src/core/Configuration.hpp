#ifndef __CONFIGURATION_HPP__
#define __CONFIGURATION_HPP__

#include <string>

#include <opencv2/core/core.hpp>

#include "../math/Quaternion.hpp"

class Options{

    public:
        
        Options( const std::string& config_file );
        
        void print()const;
        
        cv::Mat image;
        cv::Mat dem;
    
        std::string run_type;

        cv::Size get_build_image_size()const;
        int      get_build_image_type()const;
        
        int      get_rectify_image_type()const;
        
        double get_focal_length()const;
        void   set_focal_length( double const& fl );
        
        //position of the original camera
        cv::Mat Position_i;

        //position of the rectified camera
        cv::Mat Position_f;
        
        //rotation matrix for the quaternion
        cv::Mat RotationM;

        Quaternion RotationQ;

        cv::Mat get_build_img2cam()const;
        cv::Mat get_output_img2cam( cv::Size const& sz )const;
        cv::Mat get_build_cam2img()const;
        cv::Mat get_output_cam2img( cv::Size const& sz )const;
        
        cv::Mat get_output_cam2img( cv::Size const& sz, double const& ref )const;
        cv::Mat get_output_img2cam( cv::Size const& sz, double const& ref )const;
        
        bool doZBuffering()const;
        bool doPerspective2Parallel()const;

    private:
        
        bool perspective2parallel;
        bool zbufferEnabled;

        void load_configuration( );

        std::string image_filename;
        std::string config_filename;
        
        double focal_length;

        cv::Size build_image_size;
        std::string build_image_type;
        
        std::string rectify_image_type;
};



#endif
