/**
 * @file    Frame_Info.hpp
 * @author  Marvin Smith
 * @date    1/22/2019
 */
#ifndef OPENCV_STAB_DEMO_FRAME_INFO_HPP
#define OPENCV_STAB_DEMO_FRAME_INFO_HPP

// Boost Libraries
#include <boost/filesystem.hpp>

// OpenCV Libraries
#include <opencv2/core.hpp>
#include <opencv2/features2d.hpp>

// C++ Libraries
#include <memory>
#include <vector>

/**
 * @class Frame_Info
 */
class Frame_Info
{
    public:

        typedef std::shared_ptr<Frame_Info> ptr_t;

        /**
         * @brief Constructor
         * @param frame_number
         * @param image_path
         */
        Frame_Info( int                     frame_number,
                    boost::filesystem::path image_path );


        /**
         * @brief Check if loaded
         */
        inline bool Is_Image_Loaded()const{
            return m_image_loaded;
        }


        void Load_Image();


        /**
         * @brief Return the OpenCV Image Object
         */
        inline cv::Mat Get_Image()const{
            return m_image;
        }

    private:

        int m_frame_number;
        boost::filesystem::path m_image_path;
        bool m_image_loaded;
        cv::Mat m_image;
};

#endif
