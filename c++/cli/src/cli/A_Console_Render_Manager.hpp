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
#include "../thirdparty/ncurses/NCurses_Utilities.hpp"
#include "A_Console_Render_State.hpp"

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
        void Initialize();


        /** 
         * @brief Finalize
        */
        void Finalize();


        /**
         * @brief Refresh the Screen.
         */
        void Refresh();
        

        /**
         * @brief Wait on keyboard input.
         *
         * @return character pressed.
         */
        int Wait_Keyboard_Input();


        /**
         * @brief Update the NCurses Context.
         *
         * @param[in] ncurses_context new context to register.
         */
        virtual void Update_NCurses_Context( NCURSES::An_NCurses_Context::ptr_t context );

        
        /**
         * @brief Get the render state
         */
        inline virtual A_Console_Render_State::ptr_t Get_Render_State()const{
            return m_render_state;
        }


    protected:
        
        /**
         * @brief Print the header
         */
        void Print_Header();
        

        /**
         * @brief Print Main Context.
         */
        void Print_Main_Content();


        /**
         * @brief Print Footer
         */
        void Print_Footer();


        /**
         * @brief Print CLI
         */
        void Print_CLI();

    private:
        
        /// Class Name
        std::string m_class_name;

        /// NCurses Context
        NCURSES::An_NCurses_Context::ptr_t m_context;

        /// Render State
        A_Console_Render_State::ptr_t m_render_state;

}; // End of A_Console_Render_Manager Class

} // End of CLI Namespace


#endif
