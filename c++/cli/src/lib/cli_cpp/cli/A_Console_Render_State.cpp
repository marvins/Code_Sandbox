/**
 * @file    A_Console_Render_State.cpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#include "A_Console_Render_State.hpp"

// CLI Libraries
#include "../thirdparty/ncurses/NCurses_Utilities.hpp"

// C++ Standard Libraries
#include <iostream>


namespace CLI{

/************************************/
/*          Constructor             */
/************************************/
A_Console_Render_State::A_Console_Render_State( CLIConnectionType const& conn_type )
  : m_connection_type(conn_type),
    m_cli_prompt_text(""),
    m_cli_prompt_cursor_head(0),
    m_cli_prompt_cursor_tail(0),
    m_window_rows(0),
    m_window_cols(0),
    m_help_mode(false)
{
}


/************************/
/*      Push Text       */
/************************/
void A_Console_Render_State::Process_Input( const int& input )
{
    // SKip if < 0
    if( input < 0 ){ return; }

    // Process commands over the socket
    if( m_connection_type == CLIConnectionType::SOCKET ){
        
        // Check for backspace on socket
        if( input == 127 ){
            Apply_Backspace();
            return;
        }

        // Check for Delete Key
        if( input == KEY_DC ){
            Apply_Delete();
            return;
        }

    }

    // NCURSES
    else{

        // check for backspace
        if( input == KEY_BACKSPACE || input == 127 ){
            Apply_Backspace();
            return;
        }
    
        // check for delete
        else if( input == KEY_DC ){
            Apply_Delete();
            return;
        }

        // Check for left-key
        else if( input == KEY_LEFT ){
            Apply_Left_Key();
            return;
        }

        // Check for right-key
        else if( input == KEY_RIGHT ){
            Apply_Right_Key();
            return;
        }
    }

    // Otherwise, add character to string
    m_cli_prompt_text.push_back((char)input);

    // increment head
    m_cli_prompt_cursor_head++;
    m_cli_prompt_cursor_at++;

}


/**************************************/
/*          Set window size           */
/**************************************/
void A_Console_Render_State::Set_Window_Size( const int& rows,
                                              const int& cols )
{
    m_window_rows = rows;
    m_window_cols = cols;
}


/*********************************************/
/*          Clear the cursor text            */
/*********************************************/
void A_Console_Render_State::Clear_Cursor_Text()
{
    // clear the string
    m_cli_prompt_text = "";

    // Reset the cursors
    m_cli_prompt_cursor_head = 0;
    m_cli_prompt_cursor_tail = 0;
    m_cli_prompt_cursor_at   = 0;
}


/*******************************/
/*      Apply a backspace      */
/*******************************/
void A_Console_Render_State::Apply_Backspace()
{
        
    // Erase the current character
    if( m_cli_prompt_text.size() > 0 && m_cli_prompt_cursor_at > 0){
        
        // Avoid going into negative space
        m_cli_prompt_cursor_head = std::max( 0, m_cli_prompt_cursor_head-1);
        m_cli_prompt_cursor_at   = std::max( 0, m_cli_prompt_cursor_at-1);
        
        // Delete the character
        m_cli_prompt_text.erase( m_cli_prompt_cursor_at, 1 );
    }
}

/*******************************/
/*      Apply a Delete         */
/*******************************/
void A_Console_Render_State::Apply_Delete()
{
    // Erase the current character
    if( m_cli_prompt_text.size() > 0 && m_cli_prompt_cursor_at <= ((int)m_cli_prompt_text.size()-1))
    {
        m_cli_prompt_text.erase( m_cli_prompt_cursor_at, 1 );

        // Back up the cursor
        m_cli_prompt_cursor_head--;
    }
}


/******************************/
/*      Apply Right Key       */
/******************************/
void A_Console_Render_State::Apply_Left_Key()
{
    // Move the head left
    m_cli_prompt_cursor_at = std::max( m_cli_prompt_cursor_at-1, 0);

}

/******************************/
/*      Apply Left Key        */
/******************************/
void A_Console_Render_State::Apply_Right_Key()
{
    // Move the head right
    m_cli_prompt_cursor_at = std::min( m_cli_prompt_cursor_at+1, (int)m_cli_prompt_text.size()-1);
}


/********************************************/
/*      Process a CLI Command Result        */
/********************************************/
void A_Console_Render_State::Process_CLI_Command_Result( const CMD::A_CLI_Command_Result& result ){

    // If help
    if( result.Get_Parse_Status() == CMD::CLICommandParseStatus::CLI_HELP )
    {
        Set_Help_Mode(true);
    }

    // If back
    else if( result.Get_Parse_Status() == CMD::CLICommandParseStatus::CLI_BACK )
    {
        // If in help mode
        if( Get_Help_Mode() == true ){
            Set_Help_Mode(false);
        }

        // Otherwise, error
    }
}

} // End of CLI Namespace
