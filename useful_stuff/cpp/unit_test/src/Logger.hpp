#ifndef __SRC_TEST_LOGGER_H__
#define __SRC_TEST_LOGGER_H__

#include <iostream>
#include <string>
#include <vector>

#include "Color.hpp"

using namespace std;

class Status{

   public:
      Status();
      Status( const string& f, const int& r, const string& n = "");

      string func_name;
      string note;
      int result;

};
ostream& operator << (ostream& ostr, const Status& stat );

class Logger{

   public:

      void append( const Status& stat );
      
      void print_log( );

      vector<Status> log;
};

void print_module_header( const string& header_name );
void print_module_footer( const string& header_name );

void print_test_results(    const string& func_name, const int& result, const string& note="");
void print_subtest_results( const string& func_name, const int& result, const string& note="" );

#endif
