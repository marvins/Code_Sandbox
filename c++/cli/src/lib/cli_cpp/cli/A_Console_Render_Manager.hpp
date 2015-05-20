/**
 * @file    A_Console_Render_Manager.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CONSOLE_RENDER_MANAGER_HPP__
#define __CLI_A_CONSOLE_RENDER_MANAGER_HPP__

// C++ Standard Libraries
#include <deque>
#include <memory>
#include <string>

// CLI Libraries
#include "A_Command_History_Entry.hpp"
#include "A_Console_Render_State.hpp"
#include "../thirdparty/ncurses/NCurses_Utilities.hpp"


namespace CLI{


/**
 * @class A_Console_Render_Manager
 */
class A_Console_Render_Manager{

    public:

        
        /// Pointer Type
        typedef std::shared_ptr<A_Console_Render_Manager> ptr_t;
        
        
        /**
         * @brief Constructor
         */
        A_Console_Render_Manager();
        

        /**
         * @brief Initialize
        */
        virtual void Initialize() = 0;


        /** 
         * @brief Finalize
        */
        virtual void Finalize() = 0;


        /**
         * @brief Refresh the Screen.
         */
        virtual void Refresh() = 0;
        

        /**
         * @brief Get the render state
         */
        inline virtual A_Console_Render_State::ptr_t Get_Render_State()const{
            return m_render_state;
        }


        /**
         * @brief Wait on keyboard input.
         *
         * @return character pressed.
         */
        inline virtual int Wait_Keyboard_Input(){ return -1; }
        
        
        /**
         * @brief Update the NCurses Context.
         *
         * @param[in] ncurses_context new context to register.
         */
        virtual void Update_NCurses_Context( NCURSES::An_NCurses_Context::ptr_t context )
        {
        }

        
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
        
        /// CLI Title
        std::string m_cli_title;
        
        /// Command History
        std::deque<A_Command_History_Entry> m_command_history;

        /// Command Counter
        int m_command_counter;
        
        /// Render State
        A_Console_Render_State::ptr_t m_render_state;

    private:

        /// Class Name
        std::string m_class_name;

}; // End of A_Console_Render_Manager Class

} // End of CLI Namespace


#endif
