/**
 * @file    A_Console_Render_Manager.cpp
 * @author  Marvin Smith
 * @date    5/18/2015
 */
#include "A_Console_Render_Manager.hpp"

// C++ Standard Libraries
#include <iostream>
#include <sstream>

// CLI Libraries
#include "../utils/ANSI_Utilities.hpp"

namespace CLI{


/****************************/
/*      Constructor         */
/****************************/
A_Console_Render_Manager::A_Console_Render_Manager()
  : m_command_counter(0),
    m_class_name("A_Console_Render_Manager")
{
}

} // End of CLI Namespace

