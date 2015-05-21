/**
 * @file    NCurses_Utilities.hpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#ifndef __THRIRDPARTY_NCURSES_NCURSES_UTILITIES_HPP__
#define __THRIRDPARTY_NCURSES_NCURSES_UTILITIES_HPP__

// NCurses Libraries
#include <ncurses.h>

// C++ Standard Libraries
#include <memory>
#include <string>

namespace NCURSES{


/**
 * @class An_NCurses_Context
 *
 * @brief Single NCurses context object.
 */
class An_NCurses_Context{

    public:

        /// Pointer Type
        typedef std::shared_ptr<An_NCurses_Context> ptr_t;

        
        /**
         * @brief Constructor
         */
        An_NCurses_Context();
        
        
        /**
         * @brief Destructor
        */
        ~An_NCurses_Context();


        /// Screen Object
        SCREEN* screen;

        /// Window
        WINDOW* main_window;

        /// TTY Terminal Name
        std::string tty_terminal_name;

        /// TTY Input
        char*  tty_fd;
        FILE* tty_in;
        FILE* tty_out;

        /// Keyboard Timeout
        bool keyboard_blocking;

        /// Keyboard Timeout
        int keyboard_timeout_usec;

    private:
        
        /// Class name
        std::string m_class_name;


}; // End of An_NCurses_Context Class


/**
 * @brief Create NCurses Window
 */
void Initialize( An_NCurses_Context::ptr_t context );


/**
 * @brief Finalize NCurses Windows
 */
void Finalize( An_NCurses_Context::ptr_t context );


/**
 * @brief Abort
 */
void Abort();


} // End of NCURSES Namspace

#endif
