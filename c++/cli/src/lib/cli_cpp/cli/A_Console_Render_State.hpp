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
#include "cmd/A_CLI_Command_Result.hpp"
#include "A_Command_History_Entry.hpp"

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
        A_Console_Render_State( CLIConnectionType const& conn_type,
                                A_Command_History::ptr_t command_history );

        
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
         * @brief Get the Cursor Position
         */
        inline virtual int Get_Cursor_Pos()const{
            return m_cli_prompt_cursor_at;
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


        /**
         * @brief Process CLI Command
         *
         * @param[in] command Command to process.
         */
        void Process_CLI_Command_Result( CMD::A_CLI_Command_Result const& result );
        

    private:

        /**
         * @brief Add Backspace
         */
        void Apply_Backspace();
        

        /**
         * @brief Apply Delete
         */
        void Apply_Delete();
        

        /**
         * @brief Apply Left Key
        */
        void Apply_Left_Key();


        /**
         * @brief Apply Right Key
        */
        void Apply_Right_Key();


        /**
         * @brief Apply the Up Key.
         */
        void Apply_Up_Key();


        /**
         * @brief Apply the Down Key
         */
        void Apply_Down_Key();


        /// Connection Type
        CLIConnectionType m_connection_type;

        /// CLI Text
        std::string m_cli_prompt_text;

        /// CLI Cursor Position
        int m_cli_prompt_cursor_head;

        /// CLI Cursor Position
        int m_cli_prompt_cursor_tail;
        int m_cli_prompt_cursor_at;

        /// Window Rows
        int m_window_rows;

        /// Window Cols
        int m_window_cols;

        /// Help Mode
        bool m_help_mode;

        /// Reference to Command History
        A_Command_History::ptr_t m_command_history;

        /// Current History Index
        int m_command_history_ptr;

}; // End of A_Console_Render_State Class

} // End of CLI Namespace

#endif
