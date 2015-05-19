/**
 * @file    A_Console_Render_State.hpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#ifndef __CLI_A_CONSOLE_RENDER_STATE_HPP__
#define __CLI_A_CONSOLE_RENDER_STATE_HPP__

// C++ Standard Libraries
#include <memory>
#include <string>


namespace CLI{


/**
 * @class A_Console_Render_State
 */
class A_Console_Render_State{

    public:
        
        /// Pointer Type
        typedef std::shared_ptr<A_Console_Render_State> ptr_t;


        /**
         * @brief Constructor
         */
        A_Console_Render_State();

        
        /**
         * @brief Get the rows.
         */
        inline virtual int Get_Rows()const{
            return m_window_rows;
        }


        /**
         * @brief get the columns.
         */
        inline virtual int Get_Cols()const{
            return m_window_cols;
        }


        /**
         * @brief Get the cursor
         */
        inline std::string Get_Cursor_Text()const{
            return m_cli_prompt_text;
        }

        
        /**
         * @brief Add Text
         */
        virtual void Push_Text( const char& c );
   
        
        /**
         * @brief Set Window Size
         */
        virtual void Set_Window_Size( const int& rows,
                                      const int& cols );

    private:

        /// CLI Text
        std::string m_cli_prompt_text;

        /// CLI Cursor Position
        int m_cli_prompt_cursor_head;

        /// CLI Cursor Position
        int m_cli_prompt_cursor_tail;

        /// Window Rows
        int m_window_rows;

        /// Window Cols
        int m_window_cols;


}; // End of A_Console_Render_State Class

} // End of CLI Namespace

#endif
