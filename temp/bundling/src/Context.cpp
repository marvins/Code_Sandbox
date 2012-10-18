#include "Context.hpp"
#include "IO.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;



/**
 * Default constructor for the Context.
*/
Context::Context( const bool& newfile ){

    // create the header
    header = "#\n# Context file for Frame Analyzer.\n# Do not touch this file unless you know what you are doing.\n#";
    
    // initialize the is_new_file flag
    is_new_file = newfile;

}


bool Context::load_context( const string& filename ){

    // the first step is to make sure the context file exists. If not then exit and return false
    if( file_exists( filename ) == false ){
        cout << "WARNING: Context file does not exist." << endl;
        cout << "         Building new camera list anyhow. " << endl;
        is_new_file = true;
        clear_context();
        return false;
    }

    // the next step is to open the file and start parsing data from it
    vector<pair<string,string> > lines;

    // open the file
    ifstream fin;
    fin.open( filename.c_str());

    //read the first line and start the loop
    string line;
    getline( fin, line);

    while( !fin.eof()){
       
        // trim out the leading spaces
        string_trim( line );
        
        // ignore if the first character is a comment or the length is 0
        if( line.size() <= 0 || line[0] == '#' ){
            getline( fin, line);
            continue;
        }

        //split the line according to the = sign
        size_t idx = line.find_first_of("=");
        lines.push_back( pair<string,string>( line.substr(0, idx), line.substr(idx+1))); 
        
        //pull the next line
        getline( fin, line);
    }

    // close the file
    fin.close();

    /**
     *  This flag is used to identify when the NUMBER_OF_CAMERAS tag is reached and set.
     *  This is necessary as it allows us to check if the camera list loaded properly.
    */
    bool valid_size = false;
    int actual_size = 0;
    
    // create a container to store the camera cam_ids and the index positions
    //    this will serve us well to debug our efforts.
    vector<pair<int,string> > camera_reference_list;
    pair<int,string> temp_reference;

    /**
     * Parse all lines from the file and act on them
    */
    while( lines.size() > 0 ){

        //get the tag
        string tag = lines.back().first;

        //get the value
        string val = lines.back().second;

        // pop the item off the list
        lines.pop_back();

        /**
         * Compare the tag against every expected value.
        */

        // file creation tag
        if( tag == "FILE_CREATION" ){
            initial_timestamp = val;
        }

        // last modified tag 
        else if( tag == "LAST_MODIFIED" ){
            //do nothing
        }
        
        // Collection Type
        else if( tag == "COLLECT_TYPE" ){
            if( val == "RAW" )
                collect_type = COLLECT_RAW;
            else
                collect_type = COLLECT_NITF;
        }

        // Image Depth
        else if( tag == "IMAGE_DEPTH" ){
            image_depth = str2num<int>(val);
        }

        // Number of cameras
        else if( tag == "NUMBER_OF_CAMERAS" ){

            //once this appears, it is necessary to resize the camera list
            actual_size = str2num<int>(val);

            valid_size = true;
        }

        // Individual Camera
        else if( tag == "CAMERA" ){

            //split the string using the comma
            vector<string> components = string_split( val, ", ");
            
            // create a temp camera
            Camera temp_cam;
            temp_cam.root_directories.clear();

            // create a counter for the number of root directories
            int num_roots;
            
            //reset the reference
            temp_reference.first  = -1;
            temp_reference.second = "__NONE__";

            //for every component, split the string with the equal sign and evaluate
            for( size_t c=0; c<components.size(); c++ ){
                
                //create tag and val strings
                string compTag;
                string compVal;
                
                size_t comp_idx = components[c].find_first_of("=");
                compTag = components[c].substr(0, comp_idx );
                compVal = components[c].substr( comp_idx + 1 );
                
                //parse the expected options
                if( compTag == "CAM_ID" ){
                    
                    temp_cam.CAM_ID = compVal;
                    temp_reference.second = compVal;

                }
                else if( compTag == "IDX_POS" ){
                    temp_reference.first = str2num<int>(compVal);
                }

                // check for the number of root directories
                // TODO Remember that this is only used for validation
                else if( compTag == "NUMBER_ROOT_DIRECTORIES" ){
                    num_roots = str2num<int>(compVal); 
                }

                // check for camera root directory
                else if( compTag == "ROOT_DIR" ){
                    
                    temp_cam.root_directories.push_back(compVal);

                }
                else{
                    cout << "WARNING: Unidentified CAMERA component tag=" << compTag << " found in context file, aborting and reconfiguring." << endl;
                    clear_context();
                    return false;
                }

            }

            // now that we have parsed the camera line, lets add the camera to the root directory
            cameras.push_back( temp_cam );


        }

        //last resort
        else{
            cout << "WARNING: Unidentified tag=" << tag << " found in context file, aborting and reconfiguring." << endl;
            clear_context();
            return false;
        }



    }
    

    /**
     * We have parsed the configuration file, lets validate our findings
    */
    
    // sort by camera name
    sort( cameras.begin(), cameras.end(), CameraSortFunc() );


    return true;
}


/**
 * Writes the context to file
*/
void Context::write_context( const string& filename )const{

    /**
     * Open file
    */
    ofstream fout;
    fout.open( filename.c_str() );

    //////////////////////////////////////////////
    // write the header
    fout << header << endl;
    
    //////////////////////////////////////////////
    // write the inital timestamp
    fout << "FILE_CREATION=";
    if( is_new_file == false ) //write a new timestamp
        fout << initial_timestamp << endl;

    else //write the initial timestamp
        fout << query_datetime() << endl;
    fout << endl;

    /////////////////////////////////////////////
    // write the update timestamp
    fout << "LAST_MODIFIED=" << query_datetime() << endl;
    fout << endl;
    
    /////////////////////////////////////////////
    // write the frame depth 
    fout << "IMAGE_DEPTH=" << image_depth << endl;

    ////////////////////////////////////////////
    // write the number of cameras
    fout << "NUMBER_OF_CAMERAS=" << cameras.size() << endl;
    fout << endl;

    ////////////////////////////////////////////
    // write the collect type
    fout << "COLLECT_TYPE=";
    if( collect_type == COLLECT_RAW )
        fout << "RAW" << endl;
    else
        fout << "NITF" << endl;
    fout << endl;

    ///////////////////////////////////////////
    // for each camera, output its required parameters
    for( size_t i=0; i<cameras.size(); i++ ){

        fout << "CAMERA=";

        // output its index position
        fout << "IDX_POS=" << i << ", ";

        // output its CAM_ID
        fout << "CAM_ID=" << cameras[i].CAM_ID << ", ";

        // output the number of root directories
        fout << "NUMBER_ROOT_DIRECTORIES=" << cameras[i].root_directories.size() << ", ";
        
        // output each root directory
        if( cameras[i].root_directories.size() > 0 ){
            
            fout << "ROOT_DIR=" << cameras[i].root_directories[0];
            for( size_t j=1; j<cameras[i].root_directories.size(); j++ )
                fout << ", ROOT_DIR=" << cameras[i].root_directories[j];
        }


        fout << endl;

    }



    // close the file
    fout.close();


}


/**
 * Clean out the current context
*/
void Context::clear_context( ){

    //delete all cameras
    cameras.clear();

}

