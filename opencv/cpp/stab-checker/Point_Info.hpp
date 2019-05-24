/**
 * @file    Point_Info.hpp
 * @author  Marvin Smith
 * @date    5/23/2019
 */
#ifndef STAB_CHECKER_POINT_INFO_HPP
#define STAB_CHECKER_POINT_INFO_HPP

// OpenCV Libraries
#include <opencv2/core.hpp>

class Point_Info
{
    public:

        /**
         * @brief Constructor
         * @param pixel
         * @param color
         */
        Point_Info( cv::Point   pixel,
                    cv::Scalar  color );


        cv::Point Get_Pixel()const{
            return m_pixel;
        }

        cv::Scalar Get_Color()const{
            return m_color;
        }

    private:

        cv::Point  m_pixel;
        cv::Scalar m_color;
};

#endif