/**
 * @file    Point_Info.cpp
 * @author  Marvin Smith
 * @date    5/23/2019
 */
#include "Point_Info.hpp"


/********************************/
/*          Constructor         */
/********************************/
Point_Update::Point_Update( cv::Point offset,
                            double    distance,
                            double    angle )
  : m_offset(offset),
    m_distance(distance),
    m_angle(angle)
{
}


/********************************/
/*          Constructor         */
/********************************/
Point_Info::Point_Info( cv::Point  pixel,
                        cv::Scalar color )
  : m_pixel(std::move(pixel)),
    m_color(std::move(color))
{
}


/****************************************/
/*          Get the Offset Sum          */
/****************************************/
double Point_Info::Get_Offset_Sum() const
{
    double sum = 0;
    for( const auto& p : m_point_updates )
    {
        sum += p.second.Get_Distance();
    }
    return sum;
}

/************************************/
/*          Add Frame Point         */
/************************************/
void Point_Info::Add_Frame_Point( int       frame_number,
                                  cv::Point pixel )
{
    // Compute offset from current point
    cv::Point offset = m_pixel - pixel;
    double distance = sqrt(offset.x * offset.x + offset.y * offset.y);
    double angle    = atan2( offset.y, offset.x);

    m_point_updates[frame_number] = Point_Update( offset,
                                                  distance,
                                                  angle );
}
