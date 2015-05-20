/**
 * @file    A_Console_Render_Manager.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __CLI_A_CONSOLE_RENDER_MANAGER_HPP__
#define __CLI_A_CONSOLE_RENDER_MANAGER_HPP__

// C++ Standard Libraries
#include <memory>
#include <string>

// CLI Libraries
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
        inline std::string Get_Console_Buffer()const{
            return m_console_buffer;
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



    protected:
        
        /**
         * @brief Print the header
         */
        virtual void Print_Header();
        

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
         */
        virtual void Print_CLI();


        /// CLI Title
        std::string m_cli_title;

    private:

        /**
         * @brief Build the console buffer.
         */
        void Build_Console_Buffer();

        /// Class Name
        std::string m_class_name;

        /// Render State
        A_Console_Render_State::ptr_t m_render_state;
        
        /// Console Buffer
        std::string m_console_buffer;

}; // End of A_Console_Render_Manager Class

} // End of CLI Namespace


#endif
