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
    LOG_TRACE("End of Wait Method");
}


/************************************/
/*          Run Interface           */
/************************************/
void Interface::Run()
{
    // Setup window
    cv::namedWindow(WINDOW_NAME.c_str());
    cv::setMouseCallback(WINDOW_NAME.c_str(), &Interface::Mouse_Callback, this);

    while(m_gui_running)
    {

        // Show the current image
        imshow(WINDOW_NAME.c_str(), m_view_image );
        auto key = cv::waitKey(30);

        // Check keypress
        if( key == 27 ){ // ESC
            break;
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
            break;
    }
    refresh();

    auto c = getch();

    switch(c)
    {
        case 'n':
            if(m_current_stage == (int)Stages::INITIALIZATION){
                m_current_stage++;
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
}
