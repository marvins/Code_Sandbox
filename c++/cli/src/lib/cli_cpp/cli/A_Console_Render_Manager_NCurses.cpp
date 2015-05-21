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

    // Rebuild Print Tables
    Initialize_Command_Print_Tables();

}

/****************************************************/
/*      Initialize the NCurses Print Tables         */
/****************************************************/
void A_Console_Render_Manager_NCurses::Initialize_Command_Print_Tables(){

    // Rebuild the tables
    std::vector<std::string> titles;
    std::vector<int>  widths;

    int col_offset = 5;
    int col0_width = 10;
    int col2_width = 30;
    int col1_width = m_render_state->Get_Cols() - col0_width - col2_width - 2*col_offset;
    
    // Initialize the history print table configuration
    m_history_print_table_config = NCURSES::An_NCurses_Table_Configuration(false,false);

    // Process the main command print table
    titles.push_back("ID");      widths.push_back(col0_width);
    titles.push_back("Command"); widths.push_back(col1_width);
    titles.push_back("Status");  widths.push_back(col2_width);
    m_history_print_table = std::make_shared<NCURSES::An_NCurses_Table>( titles, widths,
                                                                         m_history_print_table_config);

    
    // Update the sizes
    col0_width = 20;
    col1_width = 30;
    col2_width = m_render_state->Get_Cols() - col0_width - col1_width - 2*col_offset;
    
    // Process Parser Command List
    titles.clear();
    widths.clear();
    titles.push_back("CLI Task");            widths.push_back(col0_width);
    titles.push_back("Valid Command Names"); widths.push_back(col1_width);
    titles.push_back("Description");         widths.push_back(col2_width);
    m_parser_command_print_table = std::make_shared<NCURSES::An_NCurses_Table>( titles, widths );
    
    // Add entries
    std::string command_list;
    std::vector<std::string> command_name_list;
    for( int i=0; i<(int)m_parser_command_list.size(); i++ ){

        // Set the Formal Name
        m_parser_command_print_table->Add_Entry( i, 0, m_parser_command_list[i].Get_Formal_Name() );

        // Set argument list
        command_list = "";
        command_name_list = m_parser_command_list[i].Get_Command_Name_List();
        for( int j=0; j<(int)command_name_list.size(); j++ ){
            command_list += command_name_list[j];
            if( j < (int)command_name_list.size()-1){
                command_list += ", ";
            }
        }
        
        m_parser_command_print_table->Add_Entry( i, 1, command_list );
        
        
        // Add Description
        m_parser_command_print_table->Add_Entry( i, 2, m_parser_command_list[i].Get_Description() );
    }
    

    // Process Command List
    titles.clear();
    widths.clear();
    
    titles.push_back("Command");        widths.push_back(col0_width);
    titles.push_back("Arguments");      widths.push_back(col1_width);
    titles.push_back("Description");    widths.push_back(col2_width);
    m_command_print_table = std::make_shared<NCURSES::An_NCurses_Table>( titles, widths );

    // Add entries
    std::vector<CMD::A_CLI_Command_Argument> argument_list;
    for( int i=0; i<(int)m_command_list.size(); i++ ){

        // Set the Formal Name
        m_command_print_table->Add_Entry( i, 0, m_command_list[i].Get_Name() );

        // Set argument list
        command_list = "";
        argument_list = m_command_list[i].Get_Argument_List();
        for( int j=0; j<(int)argument_list.size(); j++ ){
            command_list += argument_list[j].Get_Name();
            if( j < (int)argument_list.size()-1){
                command_list += ", ";
            }
        }
        
        m_command_print_table->Add_Entry( i, 1, command_list );
    
        // Add Description
        m_command_print_table->Add_Entry( i, 2, m_command_list[i].Get_Description() );
    }


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

   
    // If help requested, draw the help menu
    if( m_render_state->Get_Help_Mode() == true ){
        Print_Help_Content();
    }

    // Draw the main context
    else{
        Print_Main_Content();
    }
    
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
    
    // Set the min and max rows
    int min_row = 5;
    int max_row = m_render_state->Get_Rows() - 5;
    int offset_col = 3;

    // Print the table
    m_history_print_table->Print_Table( min_row, max_row, offset_col );

}


/****************************************/
/*        Print the Help Content        */
/****************************************/
void A_Console_Render_Manager_NCurses::Print_Help_Content()
{

    // Define our stop and start rows
    int min_row = 5;
    int max_row   = m_render_state->Get_Rows() - 5;
    
    // Define our start columns
    int offset_col = 3;

    // Table Sizes
    const int title_entry_width = m_render_state->Get_Cols() - (2*offset_col);

    // Create Header lines
    std::string header_line_row = "+";
    std::string header_data_row = "|";
    
    for( int i=0; i<title_entry_width; i++ ){ header_line_row += "-"; }
    header_data_row += UTILS::Format_String("Help Menu", title_entry_width);
    
    header_line_row += "+";
    header_data_row += "|";

    // Print Title Header
    mvprintw( min_row++, offset_col, header_line_row.c_str());
    mvprintw( min_row++, offset_col, header_data_row.c_str());
    mvprintw( min_row++, offset_col, header_line_row.c_str());

    // Print Parse Table
    m_parser_command_print_table->Print_Table( min_row, max_row/3, offset_col );
    m_command_print_table->Print_Table( max_row/3+1, max_row, offset_col );

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


/************************************************************/
/*          Add History to the Command Parser Table         */
/************************************************************/
void  A_Console_Render_Manager_NCurses::Add_Command_History( const std::string&                command_string,
                                                             const CMD::A_CLI_Command_Result&  command_result )
{
    // Increment the counter
    m_command_counter++;

    // Update the table
    m_history_print_table->Add_Entry( m_command_counter,
                                      0,
                                      UTILS::num2str(m_command_counter));

    m_history_print_table->Add_Entry( m_command_counter, 
                                      1,
                                      command_string );
    
    m_history_print_table->Add_Entry( m_command_counter, 
                                      2,
                                      command_result.Get_Parse_Status_String());

    // Append to the history
    m_command_history.push_back(A_Command_History_Entry(m_command_counter,
                command_string,
                command_result ));
}

} // End of CLI Namespace

