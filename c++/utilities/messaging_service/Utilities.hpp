/**
 * @file    Utilities.hpp
 * @author  Marvin Smith
 * @date    4/19/2017
*/
#ifndef UTILITIES_HPP
#define UTILITIES_HPP

#include <iostream>
#include <mutex>
#include <string>

#define LOG( message ) Log_Class( m_class_name, __func__, __LINE__, message )

inline void Log_Class( const std::string& class_name,
                const std::string& function_name,
                const int&         line_number,
                const std::string& message )
{
    static std::mutex lck;

    lck.lock();
    std::cout << class_name << "::" << line_number << ", " << function_name << "  MSG: " << message << std::endl;
    lck.unlock();
}


#endif
