/**
 * @file   FilesystemUtilities.cpp
 * @author Marvin Smith
 * @date   10/1/2013
 */
#include "FilesystemUtilities.hpp"


#ifndef Q_MOC_RUN
#include <boost/filesystem.hpp>
#endif

namespace fs = boost::filesystem;

using namespace std;

/**
 * Check whether or not a file exists.
*/
bool file_exists( string const& filename ){
    return fs::exists( fs::path( filename ) );
}

/**
 * Return the extension of the file
*/
string file_extension( string const& pathname ){
    return fs::extension( fs::path( pathname ) );

}

/**
 * Return the filename component of the path
*/
std::string file_basename( const std::string& filename ){
    return fs::path(filename).filename().string();
}

/**
 * Return the current working directory
*/
std::string current_working_directory(){
    return fs::current_path().string();
}

/**
 * Return the canonical path 
 */
std::string file_canonical( const string& filename ){
    return fs::canonical( fs::path(filename)).string();
}

/**
 List all files in the filesystem
*/
vector<string> file_list( const string& dirname, const bool& isRecursive ){ 

    vector<string> output; 

    // create a path to test against
    fs::path p (dirname);   

    try{
    
        // make sure the directory exists
        if (fs::exists(p))    // does p actually exist?
        {
            // if it is a file, then add and return
            if (fs::is_regular_file(p)){
                output.push_back(dirname);
                return output;
            }
            
            // if it is a directory, then iterate
            else if ( fs::is_directory(p)){
                
                for( fs::directory_iterator it=fs::directory_iterator(p); it!=fs::directory_iterator(); it++ ){
                    
                    if( isRecursive == false ){
                        output.push_back( it->path().string());
                    } else {
                        
                        // if we want to search recursively, then call again
                        vector<string> temp = file_list( fs::canonical( it->path()).string(), true );
                        output.insert( output.end(), temp.begin(), temp.end());
                    }
                }
            }

            else
                cout << p << " exists, but is neither a regular file nor a directory\n";
        }
        else{
            cout << dirname << " does not exist" << endl;
            output.clear();
            return output;
        }
    }

    catch (const fs::filesystem_error& ex)
    {
        cout << ex.what() << '\n';
    }

    return output;

}

