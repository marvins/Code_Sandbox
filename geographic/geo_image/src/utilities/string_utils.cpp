#include "string_utils.hpp"

#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>

namespace ba = boost::algorithm;
namespace fs = boost::filesystem;

using namespace std;

namespace GEO{
namespace STR{

std::string trim( std::string const& in ){
    
    string out = in;
    ba::trim( out );
    return out;
}

string file_basename( string const& pathname ){

#ifdef BOOST_LEGACY
    return fs::path(pathname).filename();//.string();
#else
    return fs::path(pathname).filename().string();
#endif

}


} // end of STR
} // end of GEO

