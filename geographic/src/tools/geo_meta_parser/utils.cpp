#include "utils.h"

#include <cstdlib>
#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>

#include <ncurses.h>

#include <sstream>

using namespace std;

Options::Options(){
    file_set = false;
    debug_level = 0;

    cursor_pos = 0;
    window_top = 0;

}

void Options::print(){

    std::cout << "Filename   : " << filename << std::endl;
    std::cout << "Debug Level: " << debug_level << std::endl;
}

int str2int( std::string const& value ){
    std::stringstream sin;
    int result;
    sin << value;
    sin >> result;
    return result;
}

std::string int2str( int const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}


std::string float2str( float const& val ){

    std::stringstream sin;
    sin << val;
    return sin.str();
}

float       str2float( std::string const& val ){

    std::stringstream sin;
    float result;
    sin << val;
    sin >> result;
    return result;

}


void get_console_size( int& x, int& y ){

    //#ifdef __APPLE__ && defined(TIOCGSIZE)
    //    struct ttysize ts;
    //    ioctl( STDIN_FILENO, TIOCGSIZE, &ts);
    //    x = (int)ts.ts_cols;
    //    y = (int)ts.ts_lines;
#ifdef TIOCGSIZE
    struct ttysize ts;
    ioctl( STDIN_FILENO, TIOCGSIZE, &ts);
    x = ts.ws_col;
    y = ts.ws_row;
#elif defined(TIOCGWINSZ)
    struct winsize ts;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
    x = ts.ws_col;
    y = ts.ws_row;
#else
    x = 80;
    y = 50;
#endif
}



