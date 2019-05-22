/**
 * @file    Options.hpp
 * @author  Marvin Smith
 * @date    10/30/2015
 */
#ifndef __OPTIONS_HPP__
#define __OPTIONS_HPP__

// C++ Libraries
#include <string>
#include <vector>


// OpenCV Libraries
#include <opencv2/core/core.hpp>


/**
 * @class Options
 */
class Options
{
    public:
        
        /**
         * @brief Constructor
         *
         * @param[in] argv
         * @param[in] argc
         */
        Options( int   argc, 
                 char* argv[] );
        

        /**
         * @brief Get the Image List.
         *
         */
        inline std::vector<std::string> Get_Image_List()const{
            return m_image_list;
        }

        
        /**
         * @brief Get the output pathname
         *
         * @return output video pathname.
         */
        inline std::string Get_Output_Pathname()const{
            return m_output_pathname;
        }


        /**
         * @brief Get the Image Size
         *
         * @return Frame Size
         */
        inline cv::Size Get_Frame_Size()const{
            return m_frame_size;
        }


        /**
         * @brief Get the Image BBox
         *
         * @return ROI.
         */
        inline cv::Rect Get_Frame_ROI()const{
            return m_frame_roi;
        }


        /**
         * @brief Get the Overlay Path Flag
         *
         * @return Overlay Path flag
         */
        inline bool Get_Overlay_Path_Flag()const{
            return m_overlay_path;
        }


        /**
         * @brief Get the Corner Offset.
         */
        inline cv::Point Get_Corner_Offset()const{
            return m_corner_offset;
        }


        /**
         * @brief Get Affine Transform
         */
        inline cv::Mat Get_Affine_Transform()const{
            return m_affine_transform;
        }


        /**
         * @brief Get the Frames Per Second.
         *
         * @return FPS
         */
        inline int Get_FPS()const{
            return m_fps;
        }

        
        /**
         * @brief Get the Number of threads.
         *
         * @return Number of threads.
         */
        inline int Get_Number_Threads()const{
            return m_num_threads;
        }

        inline bool Use_GUI()const{
            return m_use_gui;
        }


        /**
         * @brief Print Usage Instructions.
         */
        void Usage()const;


    private:
        
        /**
         * @brief Parse Command-Line
         *
         * @param[in] argc
         * @param[in] argv
         */
        void Parse_Command_Line( int argc, 
                                 char* argv[] );


        /**
         * @brief Validate Configuration.
         */
        void Validate();


        /**
         * @brief Load an image path file
         *
         * @param[in] image_list_path
         *
         * @return List of images to load.
         */
        static std::vector<std::string> Load_Image_Path_File( const std::string& image_list_path );


        /// Class Name
        std::string m_class_name;

        
        /// Program Name
        std::string m_program_name;


        /// Image List
        std::vector<std::string> m_image_list;
        

        /// Output Pathname
        std::string m_output_pathname;


        /// Frame Size
        cv::Size m_frame_size;


        /// Frame ROI
        cv::Rect m_frame_roi;
        

        /// Overlay Path Value
        bool m_overlay_path;

        
        /// Pixel Offset
        cv::Point m_corner_offset;

        
        /// Affine Parameters
        double m_image_scale;
        double m_image_rotation;
        double m_image_center_x;
        double m_image_center_y;


        /// Affine Transform
        cv::Mat m_affine_transform;


        /// Frames Per Second
        int m_fps;

        /// Number of Threads
        int m_num_threads;
        
        bool m_use_gui;

}; // End of Options Class


#endif
