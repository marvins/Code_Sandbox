/**
 * @file    An_NCurses_Table.hpp
 * @author  Marvin Smith
 * @date    5/21/2015
*/
#ifndef __CLI_CPP_LIB_THIRDPARTY_NCURSES_AN_NCURSES_TABLE_HPP__
#define __CLI_CPP_LIB_THIRDPARTY_NCURSES_AN_NCURSES_TABLE_HPP__


// C++ Standard Libraries
#include <memory>
#include <string>
#include <vector>

namespace NCURSES{

/**
 * @class An_NCurses_Table_Configuration
*/
class An_NCurses_Table_Configuration{

    public:
        
        /**
         * @brief Constructor
        */
        An_NCurses_Table_Configuration( const bool& blank_line_before_entry = true,
                                        const bool& blank_line_after_entry  = true );
        

        /**
         * @brief Check if we need to have a blank line before the entry.
         */
        inline bool Has_Blank_Line_Before_Entry()const{
            return m_blank_line_before_entry;
        }
        
        
        /**
         * @brief Check if we need to have a blank line after the entry.
         */
        inline bool Has_Blank_Line_After_Entry()const{
            return m_blank_line_after_entry;
        }

    private:
        
        /// Blank line before entry
        bool m_blank_line_before_entry;

        /// Blank line after entry
        bool m_blank_line_after_entry;

}; // End of An_NCurses_Table_Configuration Class

/**
 * @class An_NCurses_Table
 */
class An_NCurses_Table{

    public:
        

        /// Pointer Type
        typedef std::shared_ptr<An_NCurses_Table> ptr_t;

        /**
         * @brief Constructor
         */
        An_NCurses_Table( std::vector<std::string>const&        column_titles,
                          std::vector<int> const&               column_widths,
                          An_NCurses_Table_Configuration const& configuration = An_NCurses_Table_Configuration());

        
        /**
         * @brief Return a formatted string for printing.
         */
        void Print_Table( const int& min_row,
                          const int& max_row,
                          const int& min_col )const;

        /**
         * @brief Add Entry
        */
        void Add_Entry( const int& row, const int& col, const std::string& data );


    private:
        
        /// Column Titles
        std::vector<std::string> m_column_titles;

        /// Column Widths
        std::vector<int>         m_column_widths;

        /// Table Data
        std::vector<std::vector<std::string>> m_table_data;

        /// Configuration
        An_NCurses_Table_Configuration m_configuration;

}; // End of An_NCurses_Table Class

} // End of NCURSES Namespace


#endif
