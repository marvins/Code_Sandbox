#include "utilities.hpp"

#include <cstdlib>
#include <iostream>

using namespace std;



/**
 * Prints the usage instructions for the program
*/
void usage(){
    cout << " usage:  " << endl;
    exit(0);
}


/**
 * Convert a list of input variables into a vector of strings
*/
vector<string>  args2vector( const int argc, char* argv[] ){

    //create the output data
    vector<string> output;
    
    //iterate through the data and convert it
    for( int i=0; i<argc; i++ )
        output.push_back(argv[i]);
    
    //return
    return output;
}

/**
 *  Process command-line arguments into tangible flags and options.
 * 
 * @param[in] args Command-Line arguments
 * @param[in/out] configuration Config flag container
 */
void process_arguments( vector<string>const& args, Options& options ){

    //iterate through arguments evaluating flags
    for( size_t i=1; i<args.size(); i++){

        //Process Filename
        if( args[i] == "-F" ){

            // if -F, then the next item is the filename
            if( args.size() <= (i+1)){
                cout << "ERROR: Next arguement after -F must be filename" << endl;
                usage();
            }
                
            options.filename = args[i+1];
            i++;
        }

        //Process Help Flags
        else if( args[i] == "-help" || args[i] == "-h" ){
            usage();
        }

        //throw exception
        else{
            cout << "ERROR: unknown flag -> " << args[i] << endl;
            usage();
        }
    }


}


void print_header( ostream& ostr, std::vector<GEO::GS2::tre_tuple>const& data ){

    for( size_t i=0; i<data.size(); i++ )
        ostr << data[i].fst << "=" << data[i].snd << endl;

}

