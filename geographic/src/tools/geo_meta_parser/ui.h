#ifndef __SRC_TOOLS_GEO_META_PARSER_UI_H__
#define __SRC_TOOLS_GEO_META_PARSER_UI_H__

#include <string>
#include <utility>
#include <vector>

#include "utils.h"

void init_gui( Options const& configuration );

void stop_gui( Options const& configuration );

void main_menu( Options& configuration );

void process_arguments( std::vector<std::pair<std::string,std::string> >const& args, Options& configuration );

void usage( );

#endif

