#include "IO.hpp"
#include "TACID.hpp"

#include <algorithm>
#include <boost/filesystem.hpp>


using namespace std;

namespace fs = boost::filesystem;

const int  FILES = 1;
const int  DIRECTORIES = 2;
const int  ALL = 3;


ImageBundle::ImageBundle( ){

    scene_number = -1;
    data.clear(); 
}

ImageBundle::ImageBundle( const string& filename ){
    
    bool isValid;
    scene_number = TACID::scene_number( filename, isValid );
    
    data.push_back( filename );

}


ostream& operator << ( ostream& ostr, ImageBundle const& bundle ){

    ostr << "Bundle: " << bundle.scene_number << endl;
    for( size_t i=0; i<bundle.data.size(); i++ )
        ostr << "  -> " << bundle.data[i] << endl;

    return ostr;

}



void compress_bundles( deque<ImageBundle> const& bundles, Options const& options ){


    //create an output filename
    string temp_filename = "/tmp/file.zip";
    string temp_directory= "/tmp/bundles";

    //create a temporary directory
    directory_create( temp_directory );

    /**
     * We need to select a subset of the total bundles 
    */
    if( bundles.size() > options.number_bundles ){
    
        // find the max and min time
        int stride = bundles.size()/options.number_bundles;
        
        // pick the required number of bundles from the group
        for( int i=0; i<options.number_bundles; i++ ){
            
            //for each bundle, group them into a proposed directory
            string bundle_name = string("bundle_scene_") + num2str(bundles[i*stride].scene_number);
            
            //create the directory in temp
            directory_create( temp_directory + string("/") + bundle_name );
           
            //for each file in the bundle, copy it to the temp directory
            deque<string>::const_iterator it = bundles[i*stride].data.begin();

            while( it != bundles[i*stride].data.end() ){
                
                //copy the file to the temp dir
                file_copy( *it, temp_directory + string("/") + bundle_name + string("/") + file_basename(*it)  );
                it++;
            }
        }


    }
    else if( bundles.size() < options.number_bundles ){
    
        throw string("NOT IMPLEMENTED YET");
    
    }
    else
        throw string("NOT IMPLEMENTED YET");

    //call zip
    system("ls");

    return;

}

void directory_append_internal( const string& filename, deque<string>& container, const int& flag ){

    //create a path
    fs::path dirname( filename );

    //make sure it exists
    if( fs::exists( dirname ) == false )
        throw string( "ERROR: directory does not exist");

    //iterate through it and pull out all files matching the flag description
    fs::directory_iterator iter(dirname);
    fs::directory_iterator end_iter;
    
    //flag, 0 is file, 1 is directory
    for( ; iter != end_iter; iter++){
        
        string entry = (*iter).path().string();
        
        //get the file type
        if( (flag & DIRECTORIES) == DIRECTORIES && fs::is_directory( *iter ) == true ){
            container.push_front( entry );    
        }
        else if( (flag & FILES) == FILES && fs::is_regular_file( *iter ) == true ){
            container.push_front( entry );    
        }
    }

    sort( container.begin(), container.end());
}

/**
 * Create the directory specified by dirname
*/
void directory_create( std::string const& dirname ){

    fs::create_directories( fs::path( dirname ));

    return;
}

string file_basename( const string& pathname ){

    return fs::path(pathname).filename().string();
}

std::string file_extension( const std::string& pathname ){
    return fs::extension( fs::path( pathname ));
}


/** 
 * Given some pathname, remove the last element.  For example, 
 * given /home/user/file.txt -> /home/user
 * given /home/user/files/   -> /home/user
*/
std::string file_pop_rear( string const& filename ){
    
  //strip the first part
  return fs::path( filename ).parent_path( ).string();

}
    
deque<string> file_decompose_path( string const& pathname ){

    //create path
    fs::path item( pathname );
    
    //create output
    deque<string> output;

    //iterate over item
    for (fs::path::iterator it(item.begin()), it_end(item.end()); it != it_end; ++it){
        output.push_back((*it).string());    
    }

    return output;
}

/**
 * Copy the file to the destination
*/
void file_copy( string const& filename, string const& dest_filename ){

    fs::copy_file( fs::path( filename), fs::path( dest_filename ) );

}

bool is_dir( const string& name ){
    return fs::is_directory( fs::path( name ) );
}

bool is_file( const string& name ){
    return fs::is_regular_file( fs::path( name ));
}

