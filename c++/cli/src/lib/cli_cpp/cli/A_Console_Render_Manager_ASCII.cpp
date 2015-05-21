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
    
    // Build the Help Buffer
    Build_Help_General_Buffer();

}

/********************************/
/*        Finalize Curses       */
/********************************/
void A_Console_Render_Manager_ASCII::Finalize()
{
}


/****************************/
/*      Get the buffer      */
/****************************/
std::vector<std::string>& A_Console_Render_Manager_ASCII::Get_Console_Buffer(){

    // Check if help requested
    if( m_render_state->Get_Help_Mode() == true ){
        return m_help_general_buffer;
    }

    // Otherwise, return the main buffer
    return m_console_buffer;
}


/********************************/
/*      Refresh the screen      */
/********************************/
void A_Console_Render_Manager_ASCII::Refresh()
{

    // If help is selected, then use that buffer
    if( m_render_state->Get_Help_Mode() == true ){
        
        // Update the CLI materials
        Print_CLI( m_help_general_buffer );
        
        return; 
    }


    // Draw the header
    Print_Header( m_console_buffer );

    
    // Draw the main context
    Print_Main_Content();

    
    // Draw the footer
    Print_Footer();

    
    // Draw the CLI
    Print_CLI( m_console_buffer );


}


/****************************************/
/*          Print the header            */
/****************************************/
void A_Console_Render_Manager_ASCII::Print_Header( std::vector<std::string>& print_buffer )
{
    
    // Set the header
    print_buffer[0] = UTILS::ANSI_CLEARSCREEN + "     " + m_cli_title + BUFFER_NEWLINE;

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
void A_Console_Render_Manager_ASCII::Print_CLI( std::vector<std::string>& print_buffer )const
{
    // Set the buffer row
    int cli_row = m_window_rows - 2;

    // Move the cursor
    std::string output = "   ";
    output += UTILS::ANSI_GREEN + std::string("cmd: ") + UTILS::ANSI_RESET;
    output += m_render_state->Get_Cursor_Text();
    
    // Check if awaiting response
    if( m_waiting_command_response == true ){
        output += "      " + UTILS::ANSI_BLACK + UTILS::ANSI_BACK_RED + "   WAITING FOR COMMAND RESPONSE   " + UTILS::ANSI_RESET;
    }

    output += "\n\r";

    // Copy to the buffer
    print_buffer[cli_row] = output;
    

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


/*****************************************/
/*      Build the General Help Buffer    */
/*****************************************/
void A_Console_Render_Manager_ASCII::Build_Help_General_Buffer()
{

    // Define our start columns
    int offset_col = 5;
    std::string BUFFER_OFFSET( offset_col, ' ');
    
    
    // Allocate Buffer
    m_help_general_buffer.resize(m_window_rows, "\n\r");
    m_help_general_buffer[0].insert(0, UTILS::ANSI_CLEARSCREEN);

    
    // Create the main text
    Print_Header( m_help_general_buffer );

    
    // set the starting positions
    int min_row = 3;
    int max_row = m_window_rows - 5;
    int cur_row = min_row;

    // Create the header
    m_help_general_buffer[cur_row++] = BUFFER_OFFSET + "General Help Options" + BUFFER_NEWLINE;
    m_help_general_buffer[cur_row++] = BUFFER_NEWLINE;

    // Iterate over all parser commands
    for( size_t i=0; i<m_parser_command_list.size(); i++ ){

        //  make sure we don't go past
        if( cur_row > max_row ){
            break;
        }

        //  Create the row
        m_help_general_buffer[cur_row++] = m_parser_command_list[i].Get_Formal_Name() + BUFFER_NEWLINE;
        m_help_general_buffer[cur_row++] = BUFFER_NEWLINE;
    
    }


}



} // End of CLI Namespace

