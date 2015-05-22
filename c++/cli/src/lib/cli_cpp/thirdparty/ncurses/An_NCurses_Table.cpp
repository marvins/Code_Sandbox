/**
 * @file    An_NCurses_Table.cpp
 * @author  Marvin Smith
 * @date    5/21/2015
 */
#include "An_NCurses_Table.hpp"

// C++ Standard Libraries
#include <stdexcept>

// CLI Libraries
#include "NCurses_Utilities.hpp"
#include "../../utils/String_Utilities.hpp"

namespace NCURSES{

/**************************/
/*      Constructor       */
/**************************/
An_NCurses_Table_Configuration::An_NCurses_Table_Configuration( const bool& blank_line_before_entry,
                                                                const bool& blank_line_after_entry )
  :  m_blank_line_before_entry( blank_line_before_entry ),
     m_blank_line_after_entry( blank_line_after_entry )
{
}


/**************************/
/*      Constructor       */
/**************************/
An_NCurses_Table::An_NCurses_Table( const std::vector<std::string>& column_titles,
                                    const std::vector<int>&         column_widths,
                                    const An_NCurses_Table_Configuration& configuration )
 : m_column_titles(column_titles),
   m_column_widths(column_widths),
   m_table_data( column_titles.size() ),
   m_table_colors( column_titles.size() ),
   m_configuration(configuration)
{

}


/*******************************/
/*      Print the table        */
/*******************************/
void An_NCurses_Table::Print_Table( const int& min_row,
                                    const int& max_row,
                                    const int& min_col ) const
{
    // Define our current row
    int cur_row = min_row;

    // Create Header lines
    std::string header_line_row = "+";
    std::string header_data_row = "|";
    std::string blank_line_row  = "|";
    
    // Iterate over each column
    for( int tcol=0; tcol < (int)m_column_titles.size(); tcol++ ){

        // Iterate over each position in the table
        for( int c=0; c<m_column_widths[tcol]; c++ ){
            header_line_row += '-'; 
        }
        header_data_row += UTILS::Format_String( m_column_titles[tcol],
                                                 m_column_widths[tcol],
                                                 UTILS::StringAlignment::LEFT );
        blank_line_row += std::string( m_column_widths[tcol], ' ');

        // append the extra
        header_line_row += "+";
        header_data_row += "|";
        blank_line_row  += '|';

    }

    // Print Table Header
    mvprintw( cur_row++, min_col, header_line_row.c_str());
    mvprintw( cur_row++, min_col, header_data_row.c_str());
    mvprintw( cur_row++, min_col, header_line_row.c_str());

    // misc variables
    bool skip_row = false;
    int current_row_entry = 0;
    std::string current_line;

    // Iterate over main window region
    for( int row = cur_row; row < max_row; row++ ){
        
        // Check if we have data left to print.  If so, print blank lines
        if( current_row_entry >= (int)m_table_data[0].size() ){
            skip_row = true; 
        }

        // Check if we need to skip the row
        if( skip_row == true ){
            skip_row = false;
            mvprintw( row, min_col, blank_line_row.c_str() );
            continue;
        }


        // Reset the string
        mvprintw( row, min_col, "|");//current_line = "|";

        // Print the data
        for( int col=0; col<(int)m_table_data.size(); col++ ){
            
            // Set the color
            attron(COLOR_PAIR(m_table_colors[col][current_row_entry]));

            // Append column
            printw( UTILS::Format_String( m_table_data[col][current_row_entry], 
                                          m_column_widths[col],
                                          UTILS::StringAlignment::LEFT ).c_str());
            
            // Set the color
            attroff(COLOR_PAIR(m_table_colors[col][current_row_entry]));
            
            // Print the end
            printw( "|" );
        }
        
        // Print
        //mvprintw( row, min_col, current_line.c_str() );

        // make blank space underneath
        if( m_configuration.Has_Blank_Line_After_Entry() == true ){
            skip_row = true;
        }

        // Increment row entry
        current_row_entry++;
    }

    // Print the bottom row
    mvprintw( max_row, min_col, header_line_row.c_str() );

}

/********************************/
/*      Add Entry to Table      */
/********************************/
void An_NCurses_Table::Add_Entry( const int& row,
                                  const int& col,
                                  const std::string& data,
                                  const int& color_code )
{

    // Check column sizes
    if( col >= (int)m_table_data.size() ){
        throw std::runtime_error( "Problem in table.");
    }

    // Resize the rows if necessary
    if( (int)m_table_data[col].size() <= row ){
        for( int c=0; c<(int)m_table_data.size(); c++ ){
            m_table_data[c].resize( row+1, std::string(""));
            m_table_colors[c].resize( row+1, 0 );
        }
    }

    // Set the value
    m_table_data[col][row] = data;
    m_table_colors[col][row] = color_code;

}


} // End of NCURSES Namspace

