#ifndef __SRC_IO_HPP__
#define __SRC_IO_HPP__

#include <deque>
#include <string>

const int  IO_FILES_ONLY = 0;
const int  IO_DIRECTORIES_ONLY = 1;
const int  IO_ALL = 2;

void directory_append_internal( const std::string& filename, std::deque<std::string>& container, const int& flag = IO_ALL );

std::string file_basename( const std::string& pathname );

#endif

