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


// CLI Libraries
#include "CLIConnectionType.hpp"

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
        A_Console_Render_State( CLIConnectionType const& conn_type );

        
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
            return m_cli_prompt_text.substr(m_cli_prompt_cursor_tail,
                                            m_cli_prompt_cursor_head - m_cli_prompt_cursor_tail);
        }


        /**
         * @brief Clear the cursor text.
         */
        void Clear_Cursor_Text();

        
        /**
         * @brief Add Text
         */
        virtual void Process_Input( const int& input );
   
        
        /**
         * @brief Set Window Size
         */
        virtual void Set_Window_Size( const int& rows,
                                      const int& cols );

        
        /**
         * @brief Set the help mode
         */
        inline void Set_Help_Mode( const bool& help_mode ){
            m_help_mode = help_mode;
        }


        /**
         * @brief Get the help mode
         */
        inline bool Get_Help_Mode()const{
            return m_help_mode;
        }
        

    private:

        /**
         * @brief Add Backspace
         */
        void Apply_Backspace();
        

        /**
         * @brief Apply Delete
         */
        void Apply_Delete();


        /// Connection Type
        CLIConnectionType m_connection_type;

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

        /// Help Mode
        bool m_help_mode;

}; // End of A_Console_Render_State Class

} // End of CLI Namespace

#endif
