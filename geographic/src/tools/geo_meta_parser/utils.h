#ifndef __SRC_TOOLS_GEO_META_PARSER_UTILS_H__
#define __SRC_TOOLS_GEO_META_PARSER_UTILS_H__

#include <string>

class geoTuple{
    public:
        std::string id;
        std::string val;
        std::string type;
};


std::string int2str( int const& val );
int         str2int( std::string const& val );

void get_console_size( int& x, int& y );

#endif
