#include "Logger.h"

Status::Status():func_name(""),note(""), result(-1){}

Status::Status( const string& f, const int& r, const string& n):
         func_name(f), note(n), result(r){}

ostream& operator << (ostream& ostr, const Status& stat ){
   ostr << Color(BLUE) << "Function: " << Color(GREEN) << stat.func_name << Color(BLUE) << ", Result: ";
   if( stat.result == 1 )
      ostr << Color(GREEN) << "PASSED";
   else
      ostr << Color(RED) << "FAILED";
   ostr << ", " << stat.note << color_end;
   return ostr;
}


void Logger::append( const Status& stat ){
   log.push_back(stat);
}

void Logger::print_log( ){

   cout << endl;
   cout << "Log Status" << endl;
   cout << endl;
   for( size_t i=0; i<log.size(); i++)
      cout << log[i] << endl;

}

void print_module_header( const string& header_name ){
    cout << endl;
    cout << Color(BLUE) << "----------" << endl;
    cout << Color(BLUE) << "| Module : " << Color(GREEN) << header_name << endl;
    cout << Color(BLUE) << "----------" << color_end << endl;
    cout << color_end << endl;
}

void print_module_footer( const string& header_name ){
    
    cout << endl;
    cout << Color(BLUE) << " end of module : " << Color(GREEN) << header_name << endl;
    cout << color_end << endl;
}

void print_test_results( const string& func_name, const int& result, const string& note ){
    
    if(result == 1)
        cout << Color(GREEN) << "[PASSED] " << Color(BLUE) << func_name << " : " << Color(GREEN) << note << color_end << endl;
    else    
        cout << Color(RED)   << "[FAILED] " << Color(BLUE) << func_name << " : " << Color(RED)   << note << color_end << endl;
}

void print_subtest_results( const string& func_name, const int& result, const string& note ){
    
    if(result == 1)
        cout << Color(BLUE) << func_name << " : " << Color(GREEN) << note << color_end << endl;
    else    
        cout << Color(BLUE) << func_name << " : " << Color(RED)   << note << color_end << endl;
}
