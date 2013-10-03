/**
 *  @file    DataContainer.cpp
 *  @author  Marvin Smith
 *  @date    10/1/2013
 */
#include "DataContainer.hpp"

#include <src/core/FilesystemUtilities.hpp>
#include <src/core/StringUtilities.hpp>

#include <QMessageBox>

using namespace std;


    
DataContainer::DataContainer(){

    // base directory
    base_directory = current_working_directory();

    // set default configuration file
    config_filename = string(getenv("HOME"))+string("/.imagebrowser.cfg");

    indexingProgressDialogClose = false;
}

/**
 * Load DataContainer from file
*/
void DataContainer::load( int argc, char* argv[] ){

    // iterate through the input options


    // load the configuration file
#if defined(__unix) || defined(__APPLE__)
    load_unix_configuration();
#endif


}

void DataContainer::write( const string& configurationFilename )const{
    
    // set our default output filename
    string filename = config_filename;

    // if a filename was given, then overwrite it
    if( configurationFilename != "" ){
        filename = configurationFilename;
    }

    // open the file
    ofstream fout;
    fout.open(filename.c_str());

    // write the working directory
    fout << "BASE_IMAGE_DIRECTORY=" << base_directory << endl;

    // close the file
    fout.close();
    
}


void DataContainer::load_unix_configuration(){
    
    // if the base directory does not exist, we need to ask if it should
    if( file_exists(config_filename) == false ){
        
        // create dialog to check if we want to create the configuration structure
        QMessageBox msgBox;
        msgBox.setText(string(config_filename+string(" was not found.")).c_str());
        msgBox.setInformativeText("Would you like to create a configuration file?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No );
        msgBox.setDefaultButton( QMessageBox::Yes );
        int ret = msgBox.exec();

        if( ret == QMessageBox::Yes ){
            write( );
            
        }
    }

    // otherwise, we should load the configuration file
    else{
        read_file( );
    }
    
}


void DataContainer::read_file( const string& configurationFilename ){
    
    // create the output filename 
    string filename = config_filename;

    // if a config file was given, then use that
    if( configurationFilename != "" )
        filename = configurationFilename;
    

    // open the file
    ifstream fin;
    fin.open(filename.c_str());

    // read the first line
    string line;
    fin >> line;

    // split the string
    vector<string> components = string_split(line,"="); 
    
    // pass components to set function
    if( components.size() > 1 )
        setItem( components[0], components[1] );

    // iterate until empty
    while( !fin.eof() ){
        
        // grab the next line
        fin >> line;

        // set the item        
        if( components.size() > 1 )
            setItem( components[0], components[1] );
    }

    // close the file
    fin.close();

}

void DataContainer::setItem( const string& key, const string& value ){

    if( key == "BASE_IMAGE_DIRECTORY" ){
        base_directory = value;
    }
    else{
        cout << "Key: " << key << " is unknown" << endl;
    }

}

