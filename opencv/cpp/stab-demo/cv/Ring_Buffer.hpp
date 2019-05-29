/**
 * @file    Ring_Buffer.hpp
 * @author  Marvin Smith
 * @date    5/29/2019
 */

#ifndef OPENCV_VIDEOSTAB_RING_BUFFER_HPP
#define OPENCV_VIDEOSTAB_RING_BUFFER_HPP

// C++ Libraries
#include <vector>

// OpenCV Libraries
#include <opencv2/imgproc.hpp>


template <typename T> inline T& at(int idx, std::vector<T> &items)
{
    return items[cv::borderInterpolate(idx, static_cast<int>(items.size()), cv::BORDER_WRAP)];
}

template <typename T> inline const T& at(int idx, const std::vector<T> &items)
{
    return items[cv::borderInterpolate(idx, static_cast<int>(items.size()), cv::BORDER_WRAP)];
}


#endif
