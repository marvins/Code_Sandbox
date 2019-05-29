/**
 * @file    Interface.cpp
 * @author  Marvin Smith
 * @date    5/23/2019
*/
#include "Interface.hpp"

// NCurses Libraries
#include <ncurses.h>

// OpenCV Libraries
#include <opencv2/highgui.hpp>

// Project Libraries
#include "Log_Utilities.hpp"

// C++ Libraries
#include <functional>
#include <random>

const int Interface::RED_ON_WHITE;
const int Interface::BLUE_ON_WHITE;
const int Interface::BLACK_ON_WHITE;

/********************************/
/*          Constructor         */
/********************************/
Interface::Interface(Options::ptr_t options)
  : m_options(options)
{
}


/****************************/
/*          Start           */
/****************************/
void Interface::Start()
{
    // Load the Video
    using namespace cv::videostab;
    m_video_src = std::make_shared<VideoFileSource>( m_options->Get_Setting("INPUT_VIDEO_PATH") );
    m_current_image = m_video_src->nextFrame();
    m_current_image.copyTo(m_view_image);
    {
        std::stringstream sin;
        sin << "Video Stream Info\n";
        sin << " - Frames: " << m_video_src->count() << std::endl;
        sin << " - FPS   : " << m_video_src->fps() << std::endl;
        sin << " - Width : " << m_video_src->width() << std::endl;
        sin << " - Height: " << m_video_src->height() << std::endl;
        LOG_INFO(sin.str());
    }
    m_max_frames = std::min( m_video_src->count(),
                             m_options->Get_Setting_Int("MAX_FRAMES"));

    // Create GUI Window
    m_gui_thread = std::thread(&Interface::Run, this);

    // Configure NCurses
    Configure_NCurses();

    LOG_TRACE("End of Start Method.");
}


/********************************************/
/*          Wait until Completion           */
/********************************************/
void Interface::Wait_Until_Completion()
{
    while( m_gui_running )
    {
        // Render Terminal
        Render_Terminal();

        if( !m_gui_running )
        {
            clear();
            printw("Exiting");
            refresh();
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    if( m_gui_thread.joinable()){
        m_gui_thread.join();
    }
    endwin();
    nocbreak();

    // Check if we need to produce the report
    if( m_current_stage == (int)Stages::BUILD_REPORT )
    {
        Build_Report();
    }
    LOG_TRACE("End of Wait Method");
}


/************************************/
/*          Run Interface           */
/************************************/
void Interface::Run()
{
    // Setup window
    cv::namedWindow(REF_WINDOW_NAME.c_str());
    cv::namedWindow(CURRENT_WINDOW_NAME.c_str());
    cv::setMouseCallback(CURRENT_WINDOW_NAME.c_str(), &Interface::Mouse_Callback, this);

    while(m_gui_running)
    {

        // Show the current image
        m_gui_mtx.lock();
        if( m_view_image.rows > 0 )
        {
            imshow(CURRENT_WINDOW_NAME.c_str(), m_view_image);
        }
        if( m_ref_image.rows > 0 )
        {
            imshow(REF_WINDOW_NAME.c_str(), m_ref_image);
        }
        m_gui_mtx.unlock();
        auto key = cv::waitKey(30);

        // Check keypress
        if( key == 27 ){ // ESC
            break;
        }
        if( key == 'n' && m_current_stage == (int)Stages::INITIALIZATION){
            Transition_From_Init();
        }
    }

    if( !m_gui_running )
    {
        clear();
        printw("Exiting");
        refresh();
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    m_gui_running = false;
    LOG_TRACE("End of Run Method");
}


/****************************************/
/*          Configure NCurses           */
/****************************************/
void Interface::Configure_NCurses()
{
    initscr();
    start_color();
    init_pair(RED_ON_WHITE, COLOR_RED, COLOR_WHITE);
    init_pair(BLUE_ON_WHITE, COLOR_BLUE, COLOR_WHITE);
    init_pair(BLACK_ON_WHITE, COLOR_BLACK, COLOR_WHITE);
    cbreak();
    timeout(500);
}


/********************************************/
/*          Render to the Terminal          */
/********************************************/
void Interface::Render_Terminal()
{
    // Clear the terminal
    clear();

    // Print the desired window
    switch (m_current_stage)
    {
        case (int)Stages::INITIALIZATION:
            Print_Initialization_Window();
            break;

        case (int)Stages::FRAME_SELECTION:
            Print_Frame_Window();
            break;
    }
    refresh();

    auto c = getch();

    switch(c)
    {
        case 'n':
            if(m_current_stage == (int)Stages::INITIALIZATION){
                Transition_From_Init();
            }
            break;
        case 27:
            m_gui_running = false;
            break;
    }
}


/****************************************************/
/*          Print the Initialization Window         */
/****************************************************/
void Interface::Print_Initialization_Window()
{
    printw("Initial Point Selection\n");
    printw("-----------------------\n");
    printw("\n");
    printw(" Instructions:  Select points you will use to track throughout the video.\n");
    printw("\n");
    printw(" Options:\n");
    printw("  n  - Move on to frame-to-frame selection.\n");
    printw(" esc - Abort and exit.\n");
    printw("\n");
    printw("Points Selected: %d\n", m_point_list.size());
}


/****************************************************/
/*          Print the Frame-To-Frame Window         */
/****************************************************/
void Interface::Print_Frame_Window()
{
    printw("Frame %d of %d\n", m_current_frame, m_video_src->count());
    printw("\n");
    printw("Options:\n");

}


/*********************************/
/*          Mouse Callback       */
/*********************************/
void Interface::Mouse_Callback(int event, int x, int y, int flags, void *userdata)
{
    auto instance = (Interface*)userdata;

    instance->Mouse_Callback_Worker(event, x, y, flags);
}


/********************************************/
/*          Mouse Callback Worker           */
/********************************************/
void Interface::Mouse_Callback_Worker(int event, int x, int y, int flags )
{
    // If the user clicked and the GUI is in initialization mode,
    if( event == CV_EVENT_LBUTTONDOWN && m_current_stage == (int)Stages::INITIALIZATION )
    {
        Point_Info point_info( cv::Point(x,y),
                               cv::Scalar(rand()%155 + 100,
                                          rand()%155 + 100,
                                          rand()%155 + 100));

        // Create a point
        m_point_list.push_back( point_info );

        // Draw on init image
        cv::circle( m_view_image,
                    point_info.Get_Pixel(),
                    4,
                    point_info.Get_Color(),
                    2 );
    }

    // If the user clicked and the GUI is in frame selection mode.
    else if( event == CV_EVENT_LBUTTONDOWN && m_current_stage == (int)Stages::FRAME_SELECTION )
    {
        m_point_list[m_current_point].Add_Frame_Point( m_current_frame,
                                                       cv::Point(x,y));

        // Increment point
        m_current_point++;
        if( m_current_point >= m_point_list.size() )
        {
            m_current_point = 0;
            Increment_Frame(m_options->Get_Setting_Int("FRAME_SKIP_COUNT"));
        }

        // Update the Reference Box
        Draw_Boxes();
    }
}


/************************************/
/*          Increment Frame         */
/************************************/
void Interface::Increment_Frame(int number_frames)
{
    m_gui_mtx.lock();
    for( int i=0; i<number_frames; i++ )
    {
        m_current_frame++;

        // Skip if the video is at the end
        if( m_current_frame < m_max_frames )
        {
            m_current_image = m_video_src->nextFrame();
        }
        else{
            m_current_stage = (int)Stages::BUILD_REPORT;
            m_gui_running = false;
        }
    }

    m_current_image.copyTo(m_view_image);
    m_gui_mtx.unlock();
}


/****************************************************/
/*          Transition from Init to Next            */
/****************************************************/
void Interface::Transition_From_Init()
{
    m_current_stage++;
    m_view_image.copyTo(m_ref_image_orig);

    Draw_Boxes();

    // Increment frame
    Increment_Frame( m_options->Get_Setting_Int("FRAME_SKIP_COUNT"));
}


/**********************************************/
/*      Draw Reference Image Bounding Box     */
/**********************************************/
void Interface::Draw_Boxes()
{
    m_ref_image_orig.copyTo(m_ref_image );

    auto point_info = m_point_list[m_current_point];
    cv::Point offset(m_options->Get_Setting_Int("BOX_OFFSET"),m_options->Get_Setting_Int("BOX_OFFSET"));
    cv::rectangle( m_ref_image,
                   point_info.Get_Pixel()-offset,
                   point_info.Get_Pixel()+offset,
                   point_info.Get_Color(),
                   2 );
}


/********************************************/
/*          Create Output Report            */
/********************************************/
void Interface::Build_Report()
{
    // Iterate over points, computing errors from original
    double error_sum = 0;
    double error_cnt = 0;
    for( const auto& p : m_point_list )
    {
        error_sum += p.Get_Offset_Sum();
        error_cnt += p.Get_Offset_Count();
    }

    std::stringstream sin;
    sin << "Score: " << std::fixed << (error_sum/error_cnt) << std::endl;
    LOG_INFO(sin.str());

    // Build new output video showing offsets
    m_video_src->reset();

    //
}
