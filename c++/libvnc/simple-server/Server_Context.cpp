/**
 * @file    Server_Context.cpp
 * @author  Marvin Smith
 * @date    2/15/2018
 */
#include "Server_Context.hpp"

// C++ Libraries
#include <iostream>

// LibVNC Libraries
#include <rfb/keysym.h>


const int Server_Context::BLUE;
const int Server_Context::GREEN;
const int Server_Context::RED;
const int Server_Context::WHITE;

/********************************/
/*          Constructor         */
/********************************/
Server_Context::Server_Context(const Options &options)
  : m_options(options),
    m_active_selection(0)
{

}


/********************************************/
/*          Initialize the Context          */
/********************************************/
void Server_Context::Initialize()
{
    // Build the VNC Server
    std::cout << "Creating the VNC Server Session." << std::endl;
    std::cout << " - Screen Width     : " << m_options.Get_Screen_Width() << std::endl;
    std::cout << " - Screen Height    : " << m_options.Get_Screen_Height() << std::endl;
    std::cout << " - Bits Per Sample  : " << m_options.Get_Bits_Per_Sample() << std::endl;
    std::cout << " - Samples Per Pixel: " << m_options.Get_Samples_Per_Pixel() << std::endl;
    std::cout << " - Bytes Per Pixel  : " << m_options.Get_Bytes_Per_Pixel() << std::endl;
    std::cout << " - Buffer Size      : " << m_options.Get_Frame_Buffer_Size_Bytes() << std::endl;
    m_server = rfbGetScreen( nullptr, nullptr,
                             m_options.Get_Screen_Width(),
                             m_options.Get_Screen_Height(),
                             m_options.Get_Bits_Per_Sample(),
                             m_options.Get_Samples_Per_Pixel(),
                             m_options.Get_Bytes_Per_Pixel());

    // Configure Other Settings
    m_server->port = m_options.Get_Network_Port();
    m_server->screenData = (void*)this;
    m_server->ptrAddEvent = Process_Mouse_Event;
    m_server->kbdAddEvent = Process_Keyboard_Event;
    m_server->newClientHook = Process_Client_Connection;

    // Build the Frame
    m_active_frame = cv::Mat( m_options.Get_Screen_Height(),
                              m_options.Get_Screen_Width(),
                              m_options.Get_OpenCV_PType());

    // Create the color boxes
    m_color_bboxes[Server_Context::RED]   = cv::Rect( cv::Point(5, 5),
                                                      cv::Point((m_active_frame.cols/2 - 10), (m_active_frame.rows - 10)/3));

    m_color_bboxes[Server_Context::GREEN] = cv::Rect( cv::Point(5, (m_active_frame.rows - 10)/3 + 5),
                                                      cv::Point((m_active_frame.cols/2 - 10), 2 * (m_active_frame.rows - 10)/3 + 5));

    m_color_bboxes[Server_Context::BLUE]  = cv::Rect( cv::Point(5, 2*(m_active_frame.rows - 10)/3 + 10),
                                                      cv::Point((m_active_frame.cols/2 - 10), 3*(m_active_frame.rows - 10)/3 + 10));

    m_color_bboxes[Server_Context::WHITE] = cv::Rect( cv::Point((m_active_frame.cols/2 - 10) + 5, 5),
                                                      cv::Point(m_active_frame.cols - 5, m_active_frame.rows - 5));

    // Render the Initial Frame
    Render_Active_Frame();
    std::cout << "Frame Size: " << (m_active_frame.total() * m_active_frame.elemSize()) << std::endl;

    // Build the Frame Buffer
    m_server->frameBuffer = (char*)m_active_frame.data;//new char[m_options.Get_Frame_Buffer_Size_Bytes()];

    // Initialize the Server
    rfbInitServer(m_server);

}


/************************************/
/*          Run the Server          */
/************************************/
void Server_Context::Run_Server()
{
    rfbRunEventLoop(m_server, -1, FALSE);
}


/****************************************/
/*      Initialize the Active Frame     */
/****************************************/
void Server_Context::Render_Active_Frame()
{

    /// Draw Bounding Box
    for( int32_t r=0; r<m_active_frame.rows; r++ ){
        for( int32_t c=0; c<m_active_frame.cols; c++ ){
            m_active_frame.at<cv::Vec4b>(r,c) = cv::Vec4b(255, 165, 0, 255);
        }
    }

    // Draw Red Box
    for( int r=m_color_bboxes[RED].tl().y; r<m_color_bboxes[RED].br().y; r++ ){
        for( int c=m_color_bboxes[RED].tl().x; c<m_color_bboxes[RED].br().x; c++ ){
            m_active_frame.at<cv::Vec4b>(r,c) = cv::Vec4b(255, 0, 0, 255);
        }
    }

    // Draw Green Box
    for( int r=m_color_bboxes[GREEN].tl().y; r<m_color_bboxes[GREEN].br().y; r++ ){
        for( int c=m_color_bboxes[GREEN].tl().x; c<m_color_bboxes[GREEN].br().x; c++ ){
            m_active_frame.at<cv::Vec4b>(r,c) = cv::Vec4b(0, 255, 0, 255);
        }
    }

    // Draw Blue Box
    for( int r=m_color_bboxes[BLUE].tl().y; r<m_color_bboxes[BLUE].br().y; r++ ){
        for( int c=m_color_bboxes[BLUE].tl().x; c<m_color_bboxes[BLUE].br().x; c++ ){
            m_active_frame.at<cv::Vec4b>(r,c) = cv::Vec4b(0, 0, 255, 255);
        }
    }

    // Draw Render Box
    cv::Vec4b bcolor(0,0,0,0);
    switch(m_active_selection)
    {
        case 0: /// White
            bcolor = cv::Vec4b(255,255,255,255);
            break;

        case 1: /// Red
            bcolor = cv::Vec4b(255,0,0,255);
            break;

        case 2: /// Green
            bcolor = cv::Vec4b(0,255,0,255);
            break;

        case 3: /// Blue
            bcolor = cv::Vec4b(0,0,255,255);
            break;
    }

    // Draw Window
    for( int r=m_color_bboxes[WHITE].tl().y; r<m_color_bboxes[WHITE].br().y; r++ ){
        for( int c=m_color_bboxes[WHITE].tl().x; c<m_color_bboxes[WHITE].br().x; c++ ){
            m_active_frame.at<cv::Vec4b>(r,c) = bcolor;
        }
    }
}


typedef struct ClientData {
    rfbBool oldButton;
    int oldx,oldy;
} ClientData;

/****************************************************/
/*          Process Client Disconnection            */
/****************************************************/
void Process_Client_Disconnect(rfbClientPtr cl)
{
    free(cl->clientData);
    cl->clientData = NULL;
}


/****************************************************/
/*          Process New Client Connection           */
/****************************************************/
rfbNewClientAction Process_Client_Connection(rfbClientPtr cl)
{
    cl->clientData = (void*)calloc(sizeof(ClientData),1);
    cl->clientGoneHook = Process_Client_Disconnect;
    return RFB_CLIENT_ACCEPT;
}


/****************************************/
/*          Process Mouse Event         */
/****************************************/
void Process_Mouse_Event( int          buttonMask,
                          int          x,
                          int          y,
                          rfbClientPtr cl)
{
    // Convert to CV Point
    cv::Point mouse_point( x, y );

    // Grab our Sever Information
    Server_Context* instance = static_cast<Server_Context*>(cl->screen->screenData);

    // Get the Client Context Information
    ClientData* cd = (ClientData*)cl->clientData;


    // Check Bounds for Each Color
    auto bboxes = instance->Get_Color_BBoxes();

    cv::Size frame_size = instance->Get_Active_Frame_Size();

    // Check if Pressed
    if( buttonMask )
    {
        // Check if Red
        if( bboxes[Server_Context::BLUE].contains(mouse_point))
        {
            instance->Set_Active_Selection(Server_Context::BLUE);
            instance->Render_Active_Frame();
            rfbMarkRectAsModified( cl->screen, 0, 0,
                                   frame_size.width,
                                   frame_size.height);
        }

        // Check if Green
        if( bboxes[Server_Context::GREEN].contains(mouse_point))
        {
            instance->Set_Active_Selection(Server_Context::GREEN);
            instance->Render_Active_Frame();
            rfbMarkRectAsModified( cl->screen, 0, 0,
                                   frame_size.width,
                                   frame_size.height);
        }

        // Check if Red
        if( bboxes[Server_Context::RED].contains(mouse_point))
        {
            instance->Set_Active_Selection(Server_Context::RED);
            instance->Render_Active_Frame();
            rfbMarkRectAsModified( cl->screen, 0, 0,
                                   frame_size.width,
                                   frame_size.height);
        }
    }

    /*
    if(x>=0 && y>=0 && x<maxx && y<maxy) {
        if(buttonMask) {
            int i,j,x1,x2,y1,y2;

            if(cd->oldButton==buttonMask) { // draw a line
                drawline((unsigned char*)cl->screen->frameBuffer,cl->screen->paddedWidthInBytes,bpp,
                         x,y,cd->oldx,cd->oldy);
                x1=x; y1=y;
                if(x1>cd->oldx) x1++; else cd->oldx++;
                if(y1>cd->oldy) y1++; else cd->oldy++;
                rfbMarkRectAsModified(cl->screen,x,y,cd->oldx,cd->oldy);
            } else { // draw a point (diameter depends on button)
                int w=cl->screen->paddedWidthInBytes;
                x1=x-buttonMask; if(x1<0) x1=0;
                x2=x+buttonMask; if(x2>maxx) x2=maxx;
                y1=y-buttonMask; if(y1<0) y1=0;
                y2=y+buttonMask; if(y2>maxy) y2=maxy;

                for(i=x1*bpp;i<x2*bpp;i++)
                    for(j=y1;j<y2;j++)
                        cl->screen->frameBuffer[j*w+i]=(char)0xff;
                rfbMarkRectAsModified(cl->screen,x1,y1,x2,y2);
            }

            // we could get a selection like that:  rfbGotXCutText(cl->screen,"Hallo",5);

        } else
            cd->oldButton=0;

        cd->oldx=x; cd->oldy=y; cd->oldButton=buttonMask;
    }
    rfbDefaultPtrAddEvent(buttonMask,x,y,cl);*/
}


/********************************************/
/*          Process Keyboard Event          */
/********************************************/
void Process_Keyboard_Event( rfbBool      down,
                             rfbKeySym    key,
                             rfbClientPtr cl )
{
    // Log Entry
    std::cout << "Detected Keyboard Event. Down: " << std::boolalpha << down << ", Key: " << key << std::endl;

    // Grab our Sever Information
    Server_Context* instance = static_cast<Server_Context*>(cl->screen->screenData);

    // Get the Client Context Information
    ClientData* cd = (ClientData*)cl->clientData;

    cv::Size frame_size = instance->Get_Active_Frame_Size();

    if(down)
    {
        // Escape Key, Exit App
        if(key==XK_Escape) {
            rfbCloseClient(cl);
        }
        else if(key==XK_F12)
        {
            /* close down server, disconnecting clients */
            rfbShutdownServer(cl->screen, TRUE);
        }

        else if(key==XK_F11)
        {
            /* close down server, but wait for all clients to disconnect */
            rfbShutdownServer(cl->screen, FALSE);
        }

        // Set Box Color
        else if( key == XK_B || key == XK_b )
        {
            instance->Set_Active_Selection(Server_Context::BLUE);
            instance->Render_Active_Frame();
            rfbMarkRectAsModified( cl->screen, 0, 0,
                                   frame_size.width,
                                   frame_size.height);
        }
        else if( key == XK_G || key == XK_g )
        {
            instance->Set_Active_Selection(Server_Context::GREEN);
            instance->Render_Active_Frame();
            rfbMarkRectAsModified( cl->screen, 0, 0,
                                   frame_size.width,
                                   frame_size.height);
        }
        else if( key == XK_R || key == XK_r )
        {
            instance->Set_Active_Selection(Server_Context::RED);
            instance->Render_Active_Frame();
            rfbMarkRectAsModified( cl->screen, 0, 0,
                                   frame_size.width,
                                   frame_size.height);

        }
    }
}