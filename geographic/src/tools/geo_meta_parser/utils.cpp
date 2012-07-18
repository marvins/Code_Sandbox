#include "utils.h"

#include <cstdlib>
#include <cstdio>
#include <sys/ioctl.h>
#include <unistd.h>

#include <sstream>

using namespace std;

Options::Options(){
    file_set = false;
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

void get_console_size( int& x, int& y ){

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


