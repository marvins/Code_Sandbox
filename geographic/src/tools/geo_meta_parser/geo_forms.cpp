#include "geo_forms.h"
#include "utils.h"

#include <string> 

#include <ncurses.h>

using namespace std;

void print_header( std::string const& input_message, const int& start_y, const int& width, const int& height ){

    int start_x_pos = (width/2) - ((input_message.size())/2);

    mvprintw( start_y, start_x_pos, input_message.c_str()); 

}
