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
#include <string>

namespace NCURSES{


/**
 * @class An_NCurses_Context
 *
 * @brief Single NCurses context object.
 */
class An_NCurses_Context{

    public:
        
        /**
         * @brief Constructor
         */
        An_NCurses_Context();


        /// Screen Object
        SCREEN* screen;

        /// TTY Input
        FILE* tty_in;
        FILE* tty_out;

    private:
        
        /// Class name
        std::string m_class_name;


}; // End of An_NCurses_Context Class


/**
 * @brief Create NCurses Window
 */
void Initialize( An_NCurses_Context*& context );


/**
 * @brief Finalize NCurses Windows
 */
void Finalize( An_NCurses_Context*& context );



} // End of NCURSES Namspace

#endif
