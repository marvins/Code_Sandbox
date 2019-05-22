/**
 * @file    Frame_Loader.cpp
 * @author  Marvin Smith
 * @date    1/21/2019
 */
#include "Frame_Loader.hpp"

#include "Log_Utilities.hpp"

namespace bf=boost::filesystem;

/********************************/
/*          Constructor         */
/********************************/
Frame_Loader::Frame_Loader(Options options)
  : m_options(options)
{
}


/************************************************/
/*          Initialize the Frame-Loader         */
/************************************************/
void Frame_Loader::Initialize()
{
    // Grab the file list, make sure all files exist
    auto file_list = m_options.Get_File_List();
    LOG_INFO("Found " + std::to_string(file_list.size()) + " File Entries.");
    for( auto file_pair : file_list )
    {
        auto fn  = file_pair.first;
        auto pth = file_pair.second;

        if( !bf::exists(pth))
        {
            throw std::runtime_error("Frame: " + std::to_string(fn) + ", Path does not exist: " + pth.native());
        }

        m_frame_info[fn] = std::make_shared<Frame_Info>( fn, pth );
    }

    //
}


/************************************************/
/*          Finalize the Frame-Loader           */
/************************************************/
void Frame_Loader::Finalize()
{

}


/************************************************/
/*          Get the Frame-Number Set            */
/************************************************/
std::set<int> Frame_Loader::Get_Frame_Number_Set() const
{
    std::set<int> output;
    auto frame_list = m_options.Get_File_List();

    for( auto p : frame_list ){
        output.insert(p.first);
    }
    return output;
}


/************************************************/
/*          Get the Loaded Frame Info           */
/************************************************/
Frame_Info::ptr_t  Frame_Loader::Get_Loaded_Frame_Info(int frame_number) const
{
    LOG_INFO("Retrieving Frame " + std::to_string(frame_number) + " Info");
    if( m_frame_info.find(frame_number) == m_frame_info.end()){
        return nullptr;
    }

    // Load image, if not loaded
    auto frame_info = m_frame_info.find(frame_number)->second;
    if( !frame_info->Is_Image_Loaded() )
    {
        frame_info->Load_Image();
    }

    return frame_info;
}
