/**
 * @file    Interface.hpp
 * @author  Marvin Smith
 * @date    5/23/2019
 */
#ifndef STAB_CHECKER_INTERFACE_HPP
#define STAB_CHECKER_INTERFACE_HPP

// Project Libraries
#include "Options.hpp"
#include "Point_Info.hpp"

// OpenCV Libraries
#include <opencv2/videostab.hpp>

// C++ Libraries
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>


class Interface
{
    public:

        Interface( Options::ptr_t options );


        void Start();

        void Wait_Until_Completion();

        static const int RED_ON_WHITE = 1;
        static const int BLACK_ON_WHITE = 2;
        static const int BLUE_ON_WHITE = 3;
        const std::string CURRENT_WINDOW_NAME = { "Current Frame" };
        const std::string REF_WINDOW_NAME = { "Reference Imagery" };
    private:

        void Run();

        void Configure_NCurses();

        void Render_Terminal();

        void Print_Initialization_Window();

        void Print_Frame_Window();

        static void Mouse_Callback( int event, int x, int y, int flags, void* userdata );

        void Mouse_Callback_Worker( int event, int x, int y, int flags );

        void Increment_Frame( int number_frames );

        void Transition_From_Init();

        void Draw_Boxes();

        void Build_Report();

        enum class Stages : int
        {
            INITIALIZATION  = 0,
            FRAME_SELECTION = 1,
            BUILD_REPORT    = 2,
        };

        Options::ptr_t m_options;

        /** Current Stages
         *  - 0 : Point Selection
         *  - 1 : Frame-to-Frame Selection
         */
        int m_current_stage = 0;

        std::shared_ptr<cv::videostab::VideoFileSource> m_video_src;
        cv::Mat m_current_image;
        cv::Mat m_view_image;
        cv::Mat m_ref_image;
        cv::Mat m_ref_image_orig;
        int m_current_frame = 1;
        int m_current_point = 0;
        int m_max_frames = 0;


        std::condition_variable m_update_cv;
        std::mutex m_update_mtx;

        bool m_gui_running = true;
        std::thread m_gui_thread;
        std::mutex m_gui_mtx;

        // Point Database
        std::vector<Point_Info> m_point_list;
};

#endif