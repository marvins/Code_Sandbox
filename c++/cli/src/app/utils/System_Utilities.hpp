/**
 *  @file    System_Utilities.hpp
 *  @author  Marvin Smith
 *  @date    5/20/2015
 */
#ifndef __CLI_DEMO_UTILS_SYSTEM_UTILITIES_HPP__
#define __CLI_DEMO_UTILS_SYSTEM_UTILITIES_HPP__

// C++ Standard Libraries
#include <string>

/**
 * @brief Execute Generic Shell Command
 *
 * @param[in]   command Command to execute.
 * @param[out]  output  Shell output.
 * @param[in]   mode read/write access
 *
 * @return 0 for success, 1 otherwise.
 *
*/
int Execute_Command( const std::string&  command,
                     std::string&        output,
                     const std::string&  mode = "r");
                     
      
/**
 * @brief Ping
 *
 * @param[in] address Address to ping.
 * @param[in] max_attempts Number of attempts to try and ping.
 * @param[out] details Details of failure if one occurs.
 *
 * @return True if responsive, false otherwise.
 *
 * @note { I am redirecting stderr to stdout.  I would recommend 
 *         capturing this information separately.}
 */
bool Ping( const std::string& address,
           const int&         max_attempts,
           std::string&       details );
                          

#endif
