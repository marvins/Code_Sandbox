/**
 * @file    A_Console_Render_Manager_ASCII.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_Console_Render_Manager_ASCII.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>

// CLI Libraries
#include "../utils/ANSI_Utilities.hpp"
#include "../utils/String_Utilities.hpp"

namespace CLI{

const std::string BUFFER_NEWLINE = "\n\r";

/****************************/
/*      Constructor         */
/****************************/
A_Console_Render_Manager_ASCII::A_Console_Render_Manager_ASCII()
 :  A_Console_Render_Manager(),
    m_class_name("A_Console_Render_Manager_ASCII"),
    m_window_rows(0),
    m_window_cols(0)
{
    // Create new render state
    m_render_state.reset(new A_Console_Render_State(CLIConnectionType::SOCKET));
}


/****************************/
/*      Constructor         */
/****************************/
A_Console_Render_Manager_ASCII::A_Console_Render_Manager_ASCII( const int& window_rows,
                                                    const int& window_cols )
 :  m_class_name("A_Console_Render_Manager_ASCII"),
    m_window_rows(window_rows),
    m_window_cols(window_cols)
{
    // Create new render state
    m_render_state.reset(new A_Console_Render_State(CLIConnectionType::SOCKET));
}


/********************************/
/*      Initialize Curses       */
/********************************/
void A_Console_Render_Manager_ASCII::Initialize()
{

    // Create new render state
    m_render_state.reset(new A_Console_Render_State(CLIConnectionType::SOCKET));

    // Set the size
    m_render_state->Set_Window_Size( m_window_rows, 
                                     m_window_cols );

    // Build the console buffer
    Build_Console_Buffer();

}

/********************************/
/*        Finalize Curses       */
/********************************/
void A_Console_Render_Manager_ASCII::Finalize()
{
}


/********************************/
/*      Refresh the screen      */
/********************************/
void A_Console_Render_Manager_ASCII::Refresh()
{
    // Draw the header
    Print_Header();

    
    // Draw the main context
    Print_Main_Content();

    
    // Draw the footer
    Print_Footer();

    
    // Draw the CLI
    Print_CLI();


}


/****************************************/
/*          Print the header            */
/****************************************/
void A_Console_Render_Manager_ASCII::Print_Header()
{
    
    // Set the header
    m_console_buffer[0] = UTILS::ANSI_CLEARSCREEN + "     " + m_cli_title + BUFFER_NEWLINE;

}


/************************************************/
/*          Print the Main Context              */
/************************************************/
void A_Console_Render_Manager_ASCII::Print_Main_Content()
{

    // Define our stop and start rows
    int starty = 5;
    int endy   = m_render_state->Get_Rows() - 5;
    
    
    // Define our start columns
    int offset_col = 5;
    std::string BUFFER_OFFSET( offset_col, ' ');

    
    // Table Sizes
    const int cmd_entry_width   = 7;
    const int input_entry_width = m_render_state->Get_Cols() - cmd_entry_width - (2*offset_col);


    // Create Header lines
    std::string header_line_row = "+";
    std::string header_data_row = "|";
    
    
    for( int i=0; i<cmd_entry_width; i++ ){ header_line_row += "-"; }
    header_data_row += UTILS::Format_String("CMD", cmd_entry_width);
    
    header_line_row += "+";
    header_data_row += "|";

    for( int i=0; i<input_entry_width; i++ ){ header_line_row += "-"; }
    header_data_row += UTILS::Format_String("  Input", input_entry_width, UTILS::StringAlignment::LEFT);
    
    header_line_row += "+" + BUFFER_NEWLINE;
    header_data_row += "|" + BUFFER_NEWLINE;


    // Print Table Header
    m_console_buffer[starty++] = BUFFER_OFFSET + header_line_row;
    m_console_buffer[starty++] = BUFFER_OFFSET + header_data_row;
    m_console_buffer[starty++] = BUFFER_OFFSET + header_line_row;

    // Build a blank table entry line
    std::string blank_line_row  = "|" + std::string(cmd_entry_width, ' ') + "|" + std::string(input_entry_width,' ') + "|" + BUFFER_NEWLINE;
    
    // Iterate over main window region
    int row_id = 0;
    std::string row_data;
    bool skip_row = false;
    for( int row = starty; row <= endy; row++, row_id++ )
    {
        
        // If we need to skip the row
        if( skip_row == true ){ 
            m_console_buffer[row] = BUFFER_OFFSET + blank_line_row;
            continue;
        }


        // Check if we still have commands to print
        else if( row_id < (int)m_command_history.size() ){
        
            // Creatde new row string
            row_data = "|" + UTILS::Format_String( UTILS::num2str<int>( m_command_history[row_id].Get_Command_ID()),
                                                   cmd_entry_width );
            row_data += "|" + UTILS::Format_String( "  " + m_command_history[row_id].Get_Command_String(),
                                                    input_entry_width,
                                                    UTILS::StringAlignment::LEFT );
            row_data += "|";

            // Print
            m_console_buffer[row] = BUFFER_OFFSET + row_data + BUFFER_NEWLINE;
        }


        // Otherwise, print a blank line
        else{
            m_console_buffer[row] = BUFFER_OFFSET + blank_line_row;
            continue;
        }

    }

    // Print bottom row
    m_console_buffer[endy+1] = BUFFER_OFFSET + header_line_row;


}


/****************************************/
/*          Print the Footer            */
/****************************************/
void A_Console_Render_Manager_ASCII::Print_Footer()
{


}


/********************************/
/*          Print the CLI       */
/********************************/
void A_Console_Render_Manager_ASCII::Print_CLI()
{
    // Set the buffer row
    int cli_row = m_window_rows - 2;

    // Move the cursor
    std::string output = "   ";
    output += UTILS::ANSI_GREEN + std::string("cmd: ") + UTILS::ANSI_RESET;
    output += m_render_state->Get_Cursor_Text() + "\n\r";

    // Copy to the buffer
    m_console_buffer[cli_row] = output;
    

}


/****************************************/
/*      Build the Console Buffer        */
/****************************************/
void A_Console_Render_Manager_ASCII::Build_Console_Buffer()
{
    // Allocate buffer
    m_console_buffer.resize(m_window_rows, "\n\r");


    m_console_buffer[0].insert(0, UTILS::ANSI_CLEARSCREEN);


}


} // End of CLI Namespace

