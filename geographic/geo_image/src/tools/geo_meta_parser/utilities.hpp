#ifndef __SRC_TOOLS_GEOMETAPARSER_UTILITIES_HPP__
#define __SRC_TOOLS_GEOMETAPARSER_UTILITIES_HPP__

#include <iostream>
#include <string>
#include <utility>
#include <vector>



/**
 * @class Options 
 * 
 * Contains the global variables and requirements for the system.
*/
class Options{

    public:
        
        std::string filename; /*<  Filename of image */


};

/**
 * Print the usage instructions for the program
*/
void usage();


/**
 * Convert the input arguments given in main into a vector of strings
*/
std::vector<std::string> args2vector( const int argc, char * argv[] );


/**
 * Process the command-line arguments into actionable parameters. 
 *
 * @param[in] args Command-Line arguments
 * @param[in/out] options Parameter container
*/
void process_arguments( std::vector<std::string>const& args, Options& options );


void print_header( std::ostream& ostr, const std::vector<std::pair<std::string,std::string> >& data );



#endif

