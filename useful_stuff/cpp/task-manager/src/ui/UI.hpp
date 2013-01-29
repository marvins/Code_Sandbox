#ifndef __SRC_UI_UI_HPP__
#define __SRC_UI_UI_HPP__

#include <string>

#include "../core/Options.hpp"

using namespace std;

extern Options options;

void start_ui( );

void stop_ui( );

void user_interface( );

void print_header(string const& title, int const& maxX );

void print_table( const int& maxX, const int& maxY, const int& offY );

void print_footer( const int& type,  int const& maxX, int const& maxY );

void create_task( const bool& GUI = true );

#endif
