/**
 * @file    Options.hpp
 * @author  Marvin Smith
 * @date    2/15/2018
 */
#ifndef SIMPLE_SERVER_OPTIONS_HPP
#define SIMPLE_SERVER_OPTIONS_HPP

// C++ Libraries
#include <cinttypes>
#include <string>


/**
 * @class Options
 */
class Options
{
    public:

        /**
         * @brief Parse the Command-Line
         */
        Options( int argc, char* argv[] );
        

        /**
         * @brief Print usage instructions
        */
        void Usage();


        /**
         * @brief Get Screen Width
         */
        inline int32_t Get_Screen_Width()const{
            return m_screen_width;
        }


        /**
         * @brief Get Screen Height
         */
        inline int32_t Get_Screen_Height()const{
            return m_screen_height;
        }

        
        /**
         * @brief Get the Number of Pits Per Channel.
         */
        inline int32_t Get_Bits_Per_Sample()const{
            return m_bits_per_sample;
        }


        /**
         * @brief Get the Number of Samples Per Pixel
         */
        inline int32_t Get_Samples_Per_Pixel()const{
            return m_samples_per_pixel;
        }


        /**
         * @brief Get the Number of Bytes Per Pixel.
         */
        inline int32_t Get_Bytes_Per_Pixel()const{
            return m_bytes_per_pixel;
        }


        inline int32_t Get_Frame_Buffer_Size_Bytes()const{
            return m_screen_width * m_screen_height * m_bytes_per_pixel;
        }


    private:

        /// Application Name
        std::string m_program_name;

        /// Screen Width
        int32_t m_screen_width;

        /// Screen Height
        int32_t m_screen_height;
        
        int32_t m_bits_per_sample;

        int32_t m_samples_per_pixel;

        int32_t m_bytes_per_pixel;

}; // End of Options Class

#endif
