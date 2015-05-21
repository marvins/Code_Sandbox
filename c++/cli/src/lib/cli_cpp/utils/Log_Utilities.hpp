/**
 * @file    Log_Utilities.hpp
 * @author  Marvin Smith
 * @date    5/21/2015
*/
#ifndef __CLI_CPP_UTILS_LOG_UTILITIES_HPP__
#define __CLI_CPP_UTILS_LOG_UTILITIES_HPP__


// Boost Libraries
#include <boost/log/common.hpp>
#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/sinks/sink.hpp>
#include <boost/log/sources/logger.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <boost/log/sources/record_ostream.hpp>
#include <boost/log/sources/severity_feature.hpp>
#include <boost/log/sources/severity_logger.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/trivial.hpp>

// C++ Standard Libraries
#include <string>


using namespace boost::log::trivial;


namespace UTILS{

/**
 * @brief Initialize the Logger
 */
void Initialize_Logger( const std::string& severity,
                        const std::string& log_path );



} // End of UTILS Namespace


#endif
