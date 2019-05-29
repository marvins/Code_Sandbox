/**
 * @file    Point_Info.hpp
 * @author  Marvin Smith
 * @date    5/23/2019
 */
#ifndef STAB_CHECKER_POINT_INFO_HPP
#define STAB_CHECKER_POINT_INFO_HPP

// OpenCV Libraries
#include <opencv2/core.hpp>

// C++ Libraries
#include <map>

class Point_Update
{
    public:

        Point_Update() = default;

        Point_Update( cv::Point offset,
                      double    distance,
                      double    angle );

        double Get_Distance()const{
            return m_distance;
        }

        double Get_Angle()const{
            return m_angle;
        }

    private:

        cv::Point m_offset;
        double m_distance;
        double m_angle;
};


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

        double Get_Offset_Sum()const;

        int Get_Offset_Count()const{
            return m_point_updates.size();
        }

        void Add_Frame_Point( int       frame_number,
                              cv::Point pixel );

    private:

        cv::Point  m_pixel;
        cv::Scalar m_color;

        std::map<int,Point_Update> m_point_updates;
};

#endif