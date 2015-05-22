/**
 * @file    System_Utilities.cpp
 * @author  Marvin Smith
 * @date    5/21/2015
*/
#include "System_Utilities.hpp"

// C++ Standard Libraries
#include <cxxabi.h>
#include <deque>
#include <execinfo.h>
#include <sstream>
#include <string>

// CLI Libraries
#include "Log_Utilities.hpp"

namespace UTILS{


/****************************************/
/*      Stack Trace Signal Handler      */
/****************************************/
void Stack_Trace_Handler( int exec_code )
{
    // Build stream
    std::stringstream sin;
    int max_frames = 65;

    // Print 
    sin << "stack trace:\n";

    // storage array for stack trace address data
    void* addrlist[max_frames+1];

    // retrieve current stack addresses
    int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

    if (addrlen == 0) {
        sin << "  <empty, possibly corrupt>\n";
	    return;
    }

    // resolve addresses into strings containing "filename(function+address)",
    // this array must be free()-ed
    char** symbollist = backtrace_symbols(addrlist, addrlen);

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    for (int i = 1; i < addrlen; i++)
    {
	char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

	// find parentheses and +address offset surrounding the mangled name:
	// ./module(function+0x15c) [0x8048a6d]
	for (char *p = symbollist[i]; *p; ++p)
	{
	    if (*p == '(')
		begin_name = p;
	    else if (*p == '+')
		begin_offset = p;
	    else if (*p == ')' && begin_offset) {
		end_offset = p;
		break;
	    }
	}

	if (begin_name && begin_offset && end_offset
	    && begin_name < begin_offset)
	{
	    *begin_name++ = '\0';
	    *begin_offset++ = '\0';
	    *end_offset = '\0';

	    // mangled name is now in [begin_name, begin_offset) and caller
	    // offset in [begin_offset, end_offset). now apply
	    // __cxa_demangle():

	    int status;
	    char* ret = abi::__cxa_demangle(begin_name,
					    funcname, &funcnamesize, &status);
	    if (status == 0) {
		funcname = ret; // use possibly realloc()-ed string
		    sin << symbollist[i] << " : " << funcname << "+" << begin_offset << std::endl;
	    }
	    else {
		    // demangling failed. Output function name as a C function with
		    // no arguments.
		    sin << "  " << symbollist[i] << " : " << begin_name << "() + " << begin_offset << std::endl;
	    }
	}
	else
	{
	    // couldn't parse the line? print the whole line.
	    sin << " " << symbollist[i] << std::endl;
	}
    }

    free(funcname);
    free(symbollist);

    // Log Output 
    BOOST_LOG_TRIVIAL(fatal) << sin.str();

} 

} // End of UTILS Namespace

