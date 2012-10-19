#include "Context.hpp"
#include "IO.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include <boost/algorithm/string.hpp>

namespace ba = boost::algorithm;

using namespace std;


void printChars( ostream& ostr, int numCh, char chOut ){


    // check for number of characters greater than zero
    if( numCh > 0 ){

        // output the character
        ostr << chOut;

        // repeat the process for the remaining characters
        printChars( ostr, numCh - 1, chOut );
       }
}

void printString( ostream& ostr, const string &stringVal, int blockSize, const string &justify )
{
    // initialize function/variables
    const char SPACE = ' ';
    int preSpace = 0, postSpace = 0;
    int length = stringVal.length();

    // check for right justification
    if( justify == "RIGHT" )
        // add front-end spaces
        preSpace = blockSize - length;

    // check for center justification
    else if( justify == "CENTER" ){

        // add spaces on both ends
        preSpace = ( blockSize / 2 ) - ( length / 2 );
        postSpace = blockSize - preSpace - length;
    }

    // otherwise, assume left justification
    // default if not "RIGHT" or "CENTER"
    else 
       {
        // add back-end spaces
        postSpace = blockSize - length;
       }

    // print front-end spaces, if any
    printChars( ostr, preSpace, SPACE );

    // print the string
    ostr << stringVal;

    // print back-end spaces, if any
    printChars( ostr, postSpace, SPACE );
}




/**
 * Default constructor for the Context.
*/
Context::Context( const bool& newfile ){

    // create the header
    header = "#\n# Context file for Frame Analyzer.\n# Do not touch this file unless you know what you are doing.\n#";
    
    // initialize the is_new_file flag
    is_new_file = newfile;

}


/**
 * Load the context file and import all relevant settings and history. 
 * The most important aspect of this file is that its accurate and loads
 * the scene number PERFECTLY.  The metrics class can merge metrics objects
 * with overlapping scenes, so keep that in mind. 
*/
bool Context::load_context( const string& filename ){

    // the first step is to make sure the context file exists. If not then exit and return false
    if( file_exists( filename ) == false ){
        cout << "WARNING: Context file does not exist." << endl;
        cout << "         Building new camera list anyhow. " << endl;
        is_new_file = true;
        clear_context();
        return false;
    }

    //create a temporary container for our computed results
    deque<SceneID> temp_scenes;

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

        }
        
        //context
        else if( tag == "CONTEXT" ){

            //split the string useing hte comma
            vector<string> components = string_split( val, ", ");

            //for every component, split the string with the equal sign and evaluate
            for( size_t c=0; c<components.size(); c++ ){

                string compTag, compVal;
                size_t comp_idx = components[c].find_first_of("=");
                compTag = components[c].substr(0, comp_idx );
                compVal = components[c].substr( comp_idx + 1 );

                if( compTag == "LAST_SCENE" )
                    newest_file.scene_number = str2num<int>(compVal);
                
                else if( compTag == "PATH" )
                    newest_file.file_parts = file_decompose_path( compVal );
                
                else
                    throw string("ERROR: Bad argument in CONTEXT group" );
                    

            }

            newest_file.initialized = true;

        }

        // Individual Camera
        else if( tag == "CAMERA" ){

            //split the string using the comma
            vector<string> components = string_split( val, ", ");
            
            // create a temp camera
            Camera temp_cam;
            temp_cam.root_directories.clear();

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
                //    num_roots = str2num<int>(compVal); 
                }

                // check for camera root directory
                else if( compTag == "ROOT_DIR" ){
                    
                    //validate that the root directory exists
                    if( file_exists( compVal ) == false )
                        throw string(string("ERROR: ") + compVal + string("is not a valid camera root directory" ));

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
        
        else if( tag == "METRIC_NUMBER_TOTAL_FRAMES" ){
            metrics.number_total_frames = str2num<int>(val);
        }
        else if( tag == "METRIC_NUMBER_COMPLETE_FRAMES" )
            metrics.number_complete_frames = str2num<int>(val);
        else if( tag == "METRIC_NUMBER_INCOMPLETE_FRAMES" )
            metrics.number_incomplete_frames = str2num<int>(val);
        else if( tag == "SCENE" ){
            
            
            //split the string into components
            vector<string> components = string_split( val, ", ");
            
            // create a temp scene
            SceneID temp_scene;
            temp_scene.camera_idx_list.clear();
            temp_scene.scene_number = 0;

            //for every component, split the string with the equal sign and evaluate
            for( size_t c=0; c<components.size(); c++ ){

                //create tag and val strings
                string compTag;
                string compVal;
                
                size_t comp_idx = components[c].find_first_of("=");
                compTag = components[c].substr(0, comp_idx );
                compVal = components[c].substr( comp_idx + 1 );
                
                //parse the expected options
                if( compTag == "ID" ){
                    
                    temp_scene.scene_number = str2num<int>(compVal);

                }
                else if( compTag == "CAM" ){
                    
                    temp_scene.camera_idx_list.push_back(str2num<int>(compVal));
                }
            }

            temp_scenes.push_front( temp_scene);

        }
        //last resort
        else{
            cout << "WARNING: Unidentified tag=" << tag << " found in context file, aborting and reconfiguring." << endl;
            clear_context();
            return false;
        }



    }
    
    // distribute the collect type to every camera
    for(size_t i=0; i<cameras.size(); i++ )
        cameras[i].collect_type = collect_type;
    
    // distribute the camera count to the metric class
    metrics.camera_cnt = cameras.size();

    // sort cameras by camera name
    sort( cameras.begin(), cameras.end(), CameraSortFunc() );
    
    // add all scenes to the metrics
    metrics.scene_list.insert( temp_scenes.begin(), temp_scenes.end() );
    
    /**
     * We have parsed the configuration file, lets validate our findings
    */
    


    return true;
}


/**
 * Writes the context to file
*/
void Context::write_context( const string& filename )const{
    
    ///write the evaluation results to file
    write_evaluation_results( );

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
    // Write the previous positions 
    fout << "CONTEXT= LAST_SCENE=" << newest_file.scene_number << ", PATH=" << file_merge_path( newest_file.file_parts );
    fout << endl << endl;

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

    
    ///////////////////////////////////////////////
    //  Write all scene metric results
    fout << endl;
    fout << endl;
    
    fout << "METRIC_NUMBER_TOTAL_FRAMES=" << metrics.number_total_frames << endl;
    fout << "METRIC_NUMBER_COMPLETE_FRAMES=" << metrics.number_complete_frames << endl;
    fout << "METRIC_NUMBER_INCOMPLETE_FRAMES=" << metrics.number_incomplete_frames << endl;
    fout << endl;
    fout << endl;

    set<SceneID>::iterator  it = metrics.scene_list.begin();
    for(; it != metrics.scene_list.end(); it++ ){
    
        //write tag
        fout << "SCENE="; 
        
        //write scene number
        fout << " ID=" << it->scene_number;
        
        //write camera indeces
        if( it->camera_idx_list.size() > 0 ){

            fout << ",CAM=" << it->camera_idx_list[0];
            for( size_t i=1; i<it->camera_idx_list.size(); i++ )
                fout << ",CAM=" << it->camera_idx_list[i];
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


/**
 * Write the pretty formatted data 
*/
void Context::write_evaluation_results( )const{


    //write the accuracy to file
    ofstream fout;
    fout.open("output.txt");
    
    fout << "Frame Analyzer Results" << endl;
    fout << "======================" << endl;
    fout << endl;
    fout << "     Total Counts" << endl;
    fout << "----------------------" << endl;
    fout << "Total Number of Scenes: " << metrics.number_total_frames << endl;
    fout << "Total Number of Complete Frame Sets: " << metrics.number_complete_frames << endl;
    fout << "Total Number of Incomplete Frame Sets: " << metrics.number_incomplete_frames << endl;
    fout << endl;
    fout << "+-----------------------------------+" << endl;
    fout << "| Dropped Frame Counts By Camera    |" << endl;
    fout << "|------------+----------------------+" << endl;
    fout << "|    NAME    |   FRAMES DROPPED     |" << endl;
    fout << "+------------+----------------------+" << endl;
    vector<pair<string,int> > dropped_results = metrics.query_failures_by_camera( );
    for( size_t i=0; i<dropped_results.size(); i++ )
        if( dropped_results[i].second > 0 ){
            fout << "| ";
            printString( fout, string("cam") + dropped_results[i].first, 10, "CENTER");
            fout << " | ";
            printString( fout, num2str(dropped_results[i].second), 20, "CENTER");
            fout << " |" << endl;
        }
    fout << "+------------+----------------------+" << endl;
    fout << endl << endl;
    vector<pair<int,vector<string> > > scene_results = metrics.query_failures_by_scene( );
    fout << "|  Scene    |   Number of       |    List of Cameras " << endl;
    fout << "|  Number   |   Missing Frames  |    missing Frames  " << endl;
    fout << "+-----------+-------------------+---------------------" << endl;
    for( size_t i=0; i<scene_results.size(); i++ ){


        fout << "| ";
        printString( fout, num2str(scene_results[i].first), 8, "CENTER");
        fout << "  | ";
        printString( fout, num2str(scene_results[i].second.size()), 16, "CENTER" );
        fout << "  |  ";
        if( scene_results[i].second.size() <= 0 )
            fout << " NONE" << endl;
        else{

            for( size_t j=0; j<scene_results[i].second.size(); j++ )
                fout << "cam" << scene_results[i].second[j] << ", ";
            fout << endl;
        }
    }

    fout.close();
}

