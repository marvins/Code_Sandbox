/**
 * @file    Frame_Loader.hpp
 * @author  Marvin Smith
 * @date    1/21/2019
 */
#ifndef STAB_DEMO_FRAME_LOADER_HPP
#define STAB_DEMO_FRAME_LOADER_HPP

// C++ Libraries
#include <memory>
#include <set>
#include <string>

// OpenCV Libraries
#include <opencv2/core.hpp>

// Project Libraries
#include "Frame_Info.hpp"
#include "Options.hpp"


/**
 * @class Frame_Loader
 */
class Frame_Loader
{
    public:

        Frame_Loader( Options options );


        void Initialize();


        void Finalize();


        std::set<int> Get_Frame_Number_Set()const;


        Frame_Info::ptr_t Get_Loaded_Frame_Info( int frame_number )const;

    private:

        Options m_options;

        std::map<int, Frame_Info::ptr_t> m_frame_info;
};

#endif