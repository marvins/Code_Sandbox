#include "IO.hpp"
#include "TACID.hpp"

#include <algorithm>
#include <cstdio>

#include <boost/version.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>


using namespace std;

namespace fs = boost::filesystem;
namespace ba = boost::algorithm;

const int  FILES = 1;
const int  DIRECTORIES = 2;
const int  ALL = 3;


ImageBundle::ImageBundle( ){

    scene_number = -1;
    data.clear(); 
}

ImageBundle::ImageBundle( const string& filename, const int& collectType ){
    
    bool isValid;
    scene_number = TACID::scene_number( filename, collectType, isValid );
    
    data.push_back( filename );

}


ostream& operator << ( ostream& ostr, ImageBundle const& bundle ){

    ostr << "Bundle: " << bundle.scene_number << endl;
    for( size_t i=0; i<bundle.data.size(); i++ )
        ostr << "  -> " << bundle.data[i] << endl;

    return ostr;

}



/**
 * Query the computer for a date/time value.
*/
string query_datetime(){

    //create a file to store results
    FILE* res = popen( "date", "r");

    if( !res ){
        return "NO TIME AVAILABLE";
    }

    char buffer[1024];
    char *line_p = fgets( buffer, sizeof(buffer), res);

    return string_trim(string(line_p));

}


void compress_bundles( deque<ImageBundle> const& bundles, Options const& options ){


    //create an output filename
    string temp_filename = options.output_filename + ".zip";
    string temp_directory= file_pop_rear( options.output_filename ) + "/bundles";
    string temp_directory_name="bundles";
    string temp_directory_path=file_pop_rear( options.output_filename );


    //check if files exist and delete
    if( file_exists( temp_filename ) == true ) file_delete( temp_filename );
    if( file_exists( temp_directory) == true ) 
        system(string(string("rm -rf ") + temp_directory ).c_str());

    //create a temporary directory
    directory_create( temp_directory );

    /**
     * We need to select a subset of the total bundles 
    */
    if( (int)bundles.size() >= options.number_bundles ){
    
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
                string command = string("ln -s ") + (*it) + string(" ") + temp_directory + string("/") + bundle_name + string("/") + file_basename(*it);
                system(command.c_str());      
                //file_copy( *it, temp_directory + string("/") + bundle_name + string("/") + file_basename(*it)  );
                it++;
            }
        }


    }
    else if( (int)bundles.size() < options.number_bundles ){
    
        // find the max and min time
        int stride = bundles.size()/options.number_bundles;
        
        // pick the required number of bundles from the group
        for( int i=0; (i<options.number_bundles && (i<(int)bundles.size())); i++ ){
            
            //for each bundle, group them into a proposed directory
            string bundle_name = string("bundle_scene_") + num2str(bundles[i*stride].scene_number);
            
            //create the directory in temp
            directory_create( temp_directory + string("/") + bundle_name );
           
            //for each file in the bundle, copy it to the temp directory
            deque<string>::const_iterator it = bundles[i*stride].data.begin();

            while( it != bundles[i*stride].data.end() ){
                
                //copy the file to the temp dir
                string command = string("ln -s ") + (*it) + string(" ") + temp_directory + string("/") + bundle_name + string("/") + file_basename(*it);
                system(command.c_str());      
                //file_copy( *it, temp_directory + string("/") + bundle_name + string("/") + file_basename(*it)  );
                it++;
            }
        }


    }
    else
        throw string("NOT IMPLEMENTED YET");

    //  Copy the cal file
    string cal_filename = string("/cal_files/");
    if( options.sensor_serial_found ){
        if( options.camera_type == "EO" )
            cal_filename += string("eo");
        else
            cal_filename += string("ir");
        cal_filename += string("_cal_file_") + options.sensor_serial;

        // if file does not exist, then use default all
        if( file_exists( cal_filename ) == false )
            cal_filename = "/cal_files/*";
    }
    else{
        cal_filename += "*";    
    }
    system(string(string("mkdir -p ")+temp_directory_name + string("/cal_files/")).c_str());
    system(string(string("cp ")+cal_filename+ string(" ") + temp_directory_name + string("/cal_files/")).c_str());
   
    //  copy the telmetry data
    string telem_filename = string("/dsu/central-data/collect/telemetry/");
    {
        // find the name of the telemetry file
        
        // look for it to see if it exists

        // copy the file over
        
        // otherwise copy everything


    }


    //call zip
    change_directory( temp_directory_path);
    string command = string("zip -r0 ") + temp_filename + string(" ") + temp_directory_name + " >> /dev/null";
    cout << command << endl;
    system( command.c_str());
   
    //check if files exist and delete
    if( file_exists( temp_directory) == true ){ 
        system(string(string("rm -rf ") + temp_directory ).c_str());
        cout << string(string("rm -rf ") + temp_directory ).c_str() << endl;
    }
    
    
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

#if BOOST_VERSION / 100 % 1000 < 45
    return fs::path(pathname).filename();
#else
    return fs::path(pathname).filename().string();
#endif

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
  
#if BOOST_VERSION / 100 % 1000 < 45
  //strip the first part
  return fs::path( filename ).parent_path( ).string();
#else  
  return fs::path( filename ).parent_path( ).string();
#endif

}
    
deque<string> file_decompose_path( string const& pathname ){

    //create path
    fs::path item( pathname );
    
    //create output
    deque<string> output;

    //iterate over item
    for (fs::path::iterator it(item.begin()), it_end(item.end()); it != it_end; ++it){

#if BOOST_VERSION / 100 % 1000 < 45
    output.push_back((*it));//.string());    
#else
    output.push_back((*it).string());    
#endif
    }

    return output;
}

void change_directory( const string& pathname ){

    fs::current_path( fs::path( pathname ));
}

string file_merge_path( deque<string> const& pathname ){

    if( pathname.size() <= 0 )
        return "";

    string output = "";

    if( pathname [0] == "/" )
        output = "/";
    else
        output = pathname[0] + "/";

    for( size_t i=1; i<pathname.size(); i++ ){
        output += pathname[i];
        if( i < (pathname.size()-1))
            output += "/";
    }

    return output;

}

/**
 * Check if the file exists
*/
bool file_exists( std::string const& filename ){

    return fs::exists( fs::path( filename ));
}

/**
 * Copy the file to the destination
*/
void file_copy( string const& filename, string const& dest_filename ){

    fs::copy_file( fs::path( filename), fs::path( dest_filename ) );
}

/**
 * Delete the specified file
*/
void file_delete( string const& filename ){

    //make sure file exists
    if( file_exists( filename ) == true ){
        
        // remove file
        fs::remove( fs::path( filename ));
    }

}


bool is_dir( const string& name ){
    return fs::is_directory( fs::path( name ) );
}

bool is_file( const string& name ){
    return fs::is_regular_file( fs::path( name ));
}


/**
 * Trim a string of leading and trailing spaces. 
*/
std::string string_trim( const string& str ){
    
    string tstr = str;
    boost::algorithm::trim( tstr );
    return tstr;
}

/**
 * Split a string
*/
vector<string> string_split( const string& str, const string& pattern ){

    vector<string> output;
    vector<string> substrs;
    string pre_split = str;

    // split the string according to the specified pattern
    ba::split( substrs, pre_split, ba::is_any_of(pattern));

    // iterate over each item in the substrings and fix
    for( size_t i=0; i<substrs.size(); i++ ){

        // trim the spaces and other fat from the string
        ba::trim(substrs[i]);
        if( substrs[i].size() > 0 )
            output.push_back(substrs[i]);
    }

    return output;

}

/**
 * Convert a string to lower case
*/
std::string string_toLower( const string& str ){

    string data = str;
    std::transform( data.begin(), data.end(), data.begin(), ::tolower);
    return data;
}

