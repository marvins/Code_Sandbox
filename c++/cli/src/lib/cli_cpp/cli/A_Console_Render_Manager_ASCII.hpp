/**
 * @file    A_Console_Render_Manager_ASCII.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CONSOLE_RENDER_MANAGER_ASCII_HPP__
#define __CLI_A_CONSOLE_RENDER_MANAGER_ASCII_HPP__

// C++ Standard Libraries
#include <deque>
#include <memory>
#include <string>

// CLI Libraries
#include "A_Command_History_Entry.hpp"
#include "A_Console_Render_Manager.hpp"
#include "A_Console_Render_State.hpp"
#include "../thirdparty/ncurses/NCurses_Utilities.hpp"


namespace CLI{


/**
 * @class A_Console_Render_Manager_ASCII
 */
class A_Console_Render_Manager_ASCII : public A_Console_Render_Manager {

    public:

        
        /// Pointer Type
        typedef std::shared_ptr<A_Console_Render_Manager_ASCII> ptr_t;
        
        
        /**
         * @brief Constructor
         */
        A_Console_Render_Manager_ASCII();
        

        /**
         * @brief Constructor given width and size
         */
        A_Console_Render_Manager_ASCII( const int& window_rows,
                                  const int& window_cols );
        
        /**
         * @brief Initialize
        */
        virtual void Initialize();


        /** 
         * @brief Finalize
        */
        virtual void Finalize();


        /**
         * @brief Refresh the Screen.
         */
        virtual void Refresh();
        

        /**
         * @brief Get the render state
         */
        inline virtual A_Console_Render_State::ptr_t Get_Render_State()const{
            return m_render_state;
        }


        /**
         * Brief Get the Buffer
         */
        std::vector<std::string>& Get_Console_Buffer();
        
        
        /**
         * @brief Set the Title
         *
         * @param[in] cli_title
        */
        inline virtual void Set_CLI_Title( const std::string& cli_title )
        {
            m_cli_title = cli_title;
        }


        /**
         * @brief Append Command To History.
         *
         * @param[in] command_string String representing what the user typed in.
         * @param[in] command_result Parsing and evaluation result.
         */
        inline void Add_Command_History( const std::string&                command_string,
                                         const CMD::A_CLI_Command_Result&  command_result )
        {
            m_command_history.push_back(A_Command_History_Entry(++m_command_counter,
                                                                command_string,
                                                                command_result ));
        }



    protected:
        
        /**
         * @brief Print the header
         */
        virtual void Print_Header( std::vector<std::string>& print_buffer );
        

        /**
         * @brief Print Main Context.
         */
        virtual void Print_Main_Content();


        /**
         * @brief Print Footer
         */
        virtual void Print_Footer();


        /**
         * @brief Print CLI
         *
         * @param[in] print_buffer Buffer to post cli contents to.
         */
        virtual void Print_CLI( std::vector<std::string>& print_buffer ) const;


    private:

        /**
         * @brief Build the console buffer.
         */
        void Build_Console_Buffer();


        /**
         * @brief Build the help main buffer
         */
        void Build_Help_General_Buffer();


        /// Class Name
        std::string m_class_name;


        /// Render State
        A_Console_Render_State::ptr_t m_render_state;


        /// Console Buffer
        std::vector<std::string> m_console_buffer;
        
        /// General Help Buffer
        std::vector<std::string> m_help_general_buffer;

        /// Window Size
        int m_window_rows;
        int m_window_cols;

}; // End of A_Console_Render_Manager_ASCII Class

} // End of CLI Namespace


#endif
