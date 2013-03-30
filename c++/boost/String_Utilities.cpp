#include "String_Utilities.hpp"

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>


namespace ba = boost::algorithm;
namespace fs = boost::filesystem;

using namespace std;

namespace GEO{
namespace STR{

std::string string_trim( std::string const& in ){
    
    string out = in;
    ba::trim( out );
    return out;
}

    

std::vector<string> string_split( string const& original, string const& pattern ){

    string pre_split = original;
    vector<string> substrs;

    ba::split( substrs, pre_split, ba::is_any_of(pattern));
    return substrs;
}


std::string string_toLower( const string& in ){
    
    string value = in;
    std::transform( in.begin(), in.end(), value.begin(), ::tolower);
    return value;
}

std::string string_toUpper( const string& in ){

    string value = in;
    std::transform( in.begin(), in.end(), value.begin(), ::toupper);
    return value;
}


} // end of STR
} // end of GEO

