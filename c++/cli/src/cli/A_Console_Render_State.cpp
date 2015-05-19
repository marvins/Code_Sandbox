/**
 * @file    A_Console_Render_State.cpp
 * @author  Marvin Smith
 * @date    5/19/2015
 */
#include "A_Console_Render_State.hpp"


namespace CLI{

/************************************/
/*          Constructor             */
/************************************/
A_Console_Render_State::A_Console_Render_State()
  : m_cli_prompt_text(""),
    m_cli_prompt_cursor_head(0),
    m_cli_prompt_cursor_tail(0),
    m_window_rows(0),
    m_window_cols(0)
{
}


/************************/
/*      Push Text       */
/************************/
void A_Console_Render_State::Push_Text( const char& c )
{
    // Add character to string
    m_cli_prompt_text.push_back(c);

    // increment head
    m_cli_prompt_cursor_head++;
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


} // End of CLI Namespace
