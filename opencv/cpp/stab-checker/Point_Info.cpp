/**
 * @file    Point_Info.cpp
 * @author  Marvin Smith
 * @date    5/23/2019
 */
#include "Point_Info.hpp"


/********************************/
/*          Constructor         */
/********************************/
Point_Info::Point_Info( cv::Point  pixel,
                        cv::Scalar color )
  : m_pixel(std::move(pixel)),
    m_color(std::move(color))
{
}
