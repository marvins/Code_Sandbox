#include "Options.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>

#include "FileUtilities.hpp"

Options::Options( ){

    /// set all program defaults
    set_defaults( );

}

void Options::init( int argc, char* argv[] ){
    
    /// Process Command-Line Args
    process_args( argc, argv );
    
    if( file_exists( task_directory ) == false )
        system(string(string("mkdir -p ")+task_directory).c_str());

    /// Make sure the config file exists
    if( file_exists( config_filename ) == false ){
        
        /// Write the file
        write_config_file(config_filename);
    }

    /// Otherwise read the file
    read_config_file( config_filename );
    
    /// Load all tasks
    load_tasks( task_directory );

    /// Process any non-gui modes
    if( mode == ADD_TASK ){
        new_task( args, task_directory );
        exit(0);
    } else if( mode == LIST_TASKS ){
        list_tasks( args, task_directory );
        exit(0);
    } else if( mode == VIEW_TASK ){
        //view_task( args, task_directory );
        exit(0);
    } else if( mode == DELETE_TASK ){
        //delete_task( args, task_directory );
        exit(0);
    } else if( mode == CLEAR_TASKS ){
        clear_tasks( args, task_directory );
        exit(0);
    }

}

void Options::read_config_file( const string& filename ){

    /// Temp line
    string line;
    vector<string> parts;

    ifstream fin;
    fin.open( filename.c_str());
    
    fin >> line;

    while( !fin.eof() ){

        // check line
        parts = string_split( line, "=" );

        fin >> line;
    }
    
    fin.close();

}

void Options::write_config_file( const string& filename ){

     
    // open the file
    ofstream fout;
    fout.open( filename.c_str());

    // write the location of the task file
    fout << "TASK_FILENAME=" << task_directory << endl;

    fout.close();

}

void Options::set_defaults( ){

    /// set the configuration filename
    config_filename = string(getenv("HOME")) + "/.task-manager/options.cfg";
    
    /// set the default configuration filename
    task_directory = string(getenv("HOME")) + "/.task-manager/tasks";
    
    mode = GUI;
}

void Options::load_tasks( const string& task_dir ){

    /// get the list of tasks in the directory
    vector<string> tasklist = list_directory<vector<string> >( task_dir ); 

    /// open each task
    for( size_t i=0; i<tasklist.size(); i++ ){
        tasks.push_back( Task( tasklist[i] ));
    }


}

void Options::process_args( int argc, char* argv[] ){

    for( int i=1; i<argc; i++ ){

        if(string(argv[i]) == "-c" )
            mode = ADD_TASK; 
        else if( string(argv[i]) == "-d" )
            mode = DELETE_TASK;
        else if( string(argv[i]) == "-l" )
            mode = LIST_TASKS;
        else if( string(argv[i]) == "-x" )
            mode = CLEAR_TASKS;
        else
            args.push_back( argv[i]);
    }
    
}

