#ifndef __OPENCVPLOTTOOLS_PLOTUTILS_HPP__
#define __OPENCVPLOTTOOLS_PLOTUTILS_HPP__

#include <iostream>
#include <sstream>
#include <string> 
#include <utility>
#include <vector>

using namespace std;

vector<pair<string,string> > parse_plot_tokens( string const& input_str );

template <typename TP>
TP str2num( std::string const& value ){
    
    std::stringstream sin;
    TP result;
    sin << value;
    sin >> result;
    return result;
}

template <typename TP>
std::string num2str( TP const& value ){

    std::stringstream sin;
    sin << value;
    return sin.str();
}


#endif

