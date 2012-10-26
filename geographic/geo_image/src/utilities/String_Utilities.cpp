#include "String_Utilities.hpp"

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



} // end of STR
} // end of GEO

