#include "File_Utilities.hpp"

#include <boost/filesystem.hpp>

namespace fs=boost::filesystem;

using namespace std;

bool file_exists( string const& filename ){
    return fs::exists( fs::path( filename ) );
}


string file_basename( string const& pathname ){

#ifdef BOOST_LEGACY
    return fs::path(pathname).filename();//.string();
#else
    return fs::path(pathname).filename().string();
#endif

}

