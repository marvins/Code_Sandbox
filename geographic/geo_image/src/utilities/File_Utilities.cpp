#include "File_Utilities.hpp"

#include <boost/version.hpp>
#include <boost/filesystem.hpp>

namespace fs=boost::filesystem;

using namespace std;

namespace GEO{

namespace STR{


/**
 * Check whether or not a file exists.
*/
bool file_exists( string const& filename ){
    return fs::exists( fs::path( filename ) );
}


/**
 * Return the basename of a file
*/
string file_basename( string const& pathname ){
#if ( BOOST_VERSION / 1000 - 100 ) < 4 && ( BOOST_VERSION / 100 % 1000 ) < 6
    return fs::path(pathname).filename();//.string();
#else
    return fs::path(pathname).filename().string();
#endif

}

/**
 *
 */
string file_extension( string const& pathname ){

    return fs::extension( fs::path( pathname ) );

}


}
}
