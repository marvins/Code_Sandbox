/**
 * @file    A_Console_Render_Manager_NCurses.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_Console_Render_Manager_NCurses.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>

// CLI Libraries
#include "../utils/String_Utilities.hpp"

namespace CLI{


/****************************/
/*      Constructor         */
/****************************/
A_Console_Render_Manager_NCurses::A_Console_Render_Manager_NCurses()
  : m_class_name("A_Console_Render_Manager_NCurses"),
    m_context(nullptr),
    m_render_state(std::make_shared<A_Console_Render_State>( CLIConnectionType::LOCAL ))
{
}


/****************************************/
/*      Update the NCurses Context      */
/****************************************/
void A_Console_Render_Manager_NCurses::Update_NCurses_Context( NCURSES::An_NCurses_Context::ptr_t context )
{
    m_context = context;
}

/********************************/
/*      Initialize Curses       */
/********************************/
void A_Console_Render_Manager_NCurses::Initialize()
{

    // Initialize NCurses
    NCURSES::Initialize( m_context );

    // Create new render state
    m_render_state.reset(new A_Console_Render_State( CLIConnectionType::LOCAL));

    // Set the size
    m_render_state->Set_Window_Size( LINES, COLS );

}

/********************************/
/*        Finalize Curses       */
/********************************/
void A_Console_Render_Manager_NCurses::Finalize()
{
    // Finalize NCurses
    NCURSES::Finalize( m_context );
}


/********************************/
/*      Refresh the screen      */
/********************************/
void A_Console_Render_Manager_NCurses::Refresh()
{

    // Clear the window
    clear();
    
    // Draw the header
    Print_Header();

    
    // Draw the main context
    Print_Main_Content();

    
    // Draw the footer
    Print_Footer();

    
    // Draw the CLI
    Print_CLI();


    // Refresh the window
    refresh();

}


/************************************/
/*      Wait on Keyboard Input      */
/************************************/
int A_Console_Render_Manager_NCurses::Wait_Keyboard_Input()
{

    // Wait
    return getch();
}


/****************************************/
/*          Print the header            */
/****************************************/
void A_Console_Render_Manager_NCurses::Print_Header()
{
    // header string
    std::string header_string = m_cli_title;

    // Move to the top-left corner
    mvprintw( 0, 0, header_string.c_str() );

}


/************************************************/
/*          Print the Main Context              */
/************************************************/
void A_Console_Render_Manager_NCurses::Print_Main_Content()
{
    
    // Define our stop and start rows
    int starty = 5;
    int endy   = m_render_state->Get_Rows() - 5;
    
    // Define our start columns
    int offset_col = 5;

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
    
    header_line_row += "+";
    header_data_row += "|";


    // Print Table Header
    mvprintw( starty++, offset_col, header_line_row.c_str());
    mvprintw( starty++, offset_col, header_data_row.c_str());
    mvprintw( starty++, offset_col, header_line_row.c_str());

    // Build a blank table entry line
    std::string blank_line_row  = "|" + std::string(cmd_entry_width, ' ') + "|" + std::string(input_entry_width,' ') + "|";
    
    // Iterate over main window region
    int row_id = 0;
    std::string row_data;
    bool skip_row = false;
    for( int row = starty; row <= endy; row++, row_id++ )
    {
        
        // If we need to skip the row
        if( skip_row == true ){ 
            mvprintw( row, offset_col, blank_line_row.c_str() );
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
            mvprintw( row, offset_col, row_data.c_str());
            
        }


        // Otherwise, print a blank line
        else{
            mvprintw( row, offset_col, blank_line_row.c_str() );
            continue;
        }

    }

    // Print bottom row
    mvprintw( endy+1, offset_col, header_line_row.c_str());

}


/****************************************/
/*          Print the Footer            */
/****************************************/
void A_Console_Render_Manager_NCurses::Print_Footer()
{


}


/********************************/
/*          Print the CLI       */
/********************************/
void A_Console_Render_Manager_NCurses::Print_CLI()
{
    // Set the CLI Row
    int cli_row = m_render_state->Get_Rows()-2;
    int cli_col = 2;

    // Print the Command-Line
    mvprintw( cli_row, cli_col, "cmd: " );
    cli_col += 5;
    
    // Print the cursor text
    printw( m_render_state->Get_Cursor_Text().c_str());
    
    // Set the cursor over the current character
    move( cli_row, cli_col + m_render_state->Get_Cursor_Pos() );
}


} // End of CLI Namespace

