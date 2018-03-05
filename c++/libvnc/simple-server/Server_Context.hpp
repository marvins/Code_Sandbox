/**
 * @file    Server_Context.hpp
 * @author  Marvin Smith
 * @date    2/15/2018
 */
#ifndef SIMPLE_SERVER_SERVER_CONTEXT_HPP
#define SIMPLE_SERVER_SERVER_CONTEXT_HPP

// C++ Libraries
#include <map>
#include <memory>


// Project Libraries
#include "Options.hpp"


// LibVNC Server
#include <rfb/rfb.h>


// OpenCV
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>

// For Newer versions of OpenCV
//#include <opencv2/imgcodecs/imgcodecs.hpp>


/**
 * @class Server_Context
 */
class Server_Context
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<Server_Context> ptr_t;

        /// Selection Colors
        static const int WHITE = 0;
        static const int RED   = 1;
        static const int GREEN = 2;
        static const int BLUE  = 3;

        /**
         * @brief Constructor
         */
        Server_Context( const Options& options );


        /**
         * @brief Initialize the Context
         */
        void Initialize();


        /**
         * @brief Run the Server
         */
        void Run_Server();


        /**
         * @brief Set the Active Selection
         */
        inline void Set_Active_Selection( const int active_selection ){
            m_active_selection = active_selection;
        }


        inline cv::Size Get_Active_Frame_Size()const{
            return m_active_frame.size();
        }


        /**
         * @brief Initialize the Active Image Frame
         */
        void Render_Active_Frame();


        inline std::map<int,cv::Rect> Get_Color_BBoxes()const{
            return m_color_bboxes;
        }

    private:

        /// Configuration
        Options m_options;

        /// Server Context
        rfbScreenInfoPtr m_server;


        /// Active Frame
        cv::Mat m_active_frame;


        /// Active Selection
        int m_active_selection;

        /// Color BBoxes
        std::map<int,cv::Rect> m_color_bboxes;
};


/**
 * @brief Process Client Disconnect
 * @param cl
 */
void Process_Client_Disconnect(rfbClientPtr cl);


/**
 * @brief Process a New Client Connection
 * @param cl
 * @return
 */
rfbNewClientAction Process_Client_Connection(rfbClientPtr cl);


/**
 * @brief Process Keyboard Event
 * @param down
 * @param key
 * @param cl
 */
void Process_Keyboard_Event( rfbBool      down,
                             rfbKeySym    key,
                             rfbClientPtr cl );


/**
 * @brief Process Mouse Events
 * @param buttonMask
 * @param x
 * @param y
 * @param cl
 */
void Process_Mouse_Event( int          buttonMask,
                          int          x,
                          int          y,
                          rfbClientPtr cl);

#endif
