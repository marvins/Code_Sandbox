#ifndef __WINDOW_UTILITIES_HPP__
#define __WINDOW_UTILITIES_HPP__

// C++ Libraries
#include <string>

// Include GLEW
#include <GL/glew.h>

// GLFW
#include <glfw3.h>

// GLM
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

/**
 * @class Render_Config
*/
class Render_Config
{
    public:
        
        /**
         * @brief Constructor given configuration file.
        */
        Render_Config( const std::string& config_pathname );

        /**
         * @brief Constructor
        */
        Render_Config( const int& window_cols,
                       const int& window_rows,
                       const std::string& window_name );


        /**
         * @brief Get the Window Rows.
        */
        inline int Get_Rows()const{
            return m_rows;
        }


        /**
         * @brief Get the Window Cols.
        */
        inline int Get_Cols()const{
            return m_cols;
        }


        /**
         * @brief Get the Window Name
        */
        inline std::string Get_Window_Name()const{
            return m_window_name;
        }

        /**
         * @brief Get Background Color
        */
        inline glm::vec4 Get_Background_Color()const{
            return m_background_color;
        }


    private:
        
        // Window Rows
        int m_rows;

        // Window Columns
        int m_cols;

        // Name of the window
        std::string m_window_name;
        
        /// Background Color
        glm::vec4 m_background_color;

}; // End of Render_Config Class

/**
 * @class Window
*/
class Render_Window
{
    public:

        /**
         * @brief Constructor
        */
        Render_Window( Render_Config const& configs );
        
        /**
         * @brief Initialize
        */
        void Initialize();

        /**
         * @brief Finalize
        */
        void Finalize();

    private:
        
        // Class Name
        std::string m_class_name;

        // Render Configs
        Render_Config m_config;
        
        // GLFW Window
        GLFWwindow* m_window;
        
        // Program
        GLuint m_program_id;

}; // End of Render_Window Class


#endif
