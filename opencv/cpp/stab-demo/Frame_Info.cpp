/**
 * @file    Frame_Info.cpp
 * @author  Marvin Smith
 * @date    1/22/2019
 */
#include "Frame_Info.hpp"

// Project Libraries
#include "Log_Utilities.hpp"

// OpenCV Libraries
#include <opencv2/imgcodecs.hpp>

namespace bf=boost::filesystem;

/********************************/
/*          Constructor         */
/********************************/
Frame_Info::Frame_Info( int       frame_number,
                        bf::path  image_path )
  :  m_frame_number(frame_number),
     m_image_path(image_path),
     m_image_loaded(false)
{
}


/************************************************/
/*          Load the Image into Memory          */
/************************************************/
void Frame_Info::Load_Image()
{
    LOG_INFO("Loading Image");
    m_image = cv::imread( m_image_path.native().c_str() );

    if( m_image.rows > 0 && m_image.cols > 0 ){
        m_image_loaded = true;
    }
}