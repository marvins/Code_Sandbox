#include "string_utils.hpp"

#include <boost/algorithm/string/trim.hpp>

namespace ba = boost::algorithm;
using namespace std;

namespace GEO{
namespace STR{

std::string trim( std::string const& in ){
    
    string out = in;
    ba::trim( out );
    return out;
}

} // end of STR
} // end of GEO

