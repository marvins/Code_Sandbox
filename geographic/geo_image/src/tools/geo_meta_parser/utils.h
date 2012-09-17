#ifndef __SRC_TOOLS_GEO_META_PARSER_UTILS_H__
#define __SRC_TOOLS_GEO_META_PARSER_UTILS_H__

#include <iostream>
#include <string>

#include <GeoImage.h>

const short GEO_GRAY  = 3;
const short GEO_BLACK = 0;
const short GEO_BLUE  = 4;
const short GEO_WHITE = 7;
const int   active_window_pair = 11;
const int   backg_window_pair  = 12;
const int   warning_window_pair= 13;

class geoTuple{
    public:
        std::string id;
        std::string val;
        std::string type;
};

class Options{
    public:

        Options();

        std::string filename;
        bool file_set;

        int debug_level;

        void print( );

        GEO::GeoImage current_image;

        int cursor_pos;
        int window_top;

};


std::string int2str( int const& val );
int         str2int( std::string const& val );
std::string float2str( float const& val );
float       str2float( std::string const& val );

void get_console_size( int& x, int& y );

#endif
