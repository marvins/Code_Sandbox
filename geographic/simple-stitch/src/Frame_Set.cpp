/**
 * @file    Frame_Set.cpp
 * @author  Marvin Smith
 * @date    10/27/2015
*/
#include "Frame_Set.hpp"

// Boost Libraries
#include <boost/filesystem.hpp>

namespace bf=boost::filesystem;

/****************************/
/*        Constructor       */
/****************************/
Frame_Set::Frame_Set()
 : m_class_name("Frame_Set")
{
}


/*****************************/
/*        Constructor        */
/*****************************/
Frame_Set::Frame_Set( const int& frame_num )
  : m_class_name("Frame_Set"),
    m_frame_number(frame_number )
{
}


/*********************************/
/*        Load Frame Sets        */
/*********************************/
void Load_Frame_Sets( const Options&            options,
                      std::map<int,Frame_Set>&  frame_sets )
{
    // Make sure the path exists
    if( bf::exists(options.Get_Base_Directory()) == false )
    {
        std::cerr << "Base directory (" << options.Get_Base_Directory() << ") does not exist." << std::endl;
        options.Usage();
        std::exit(-1);
    }



}


