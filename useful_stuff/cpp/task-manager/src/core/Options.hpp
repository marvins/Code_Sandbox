#ifndef __SRC_CORE_OPTIONS_HPP__
#define __SRC_CORE_OPTIONS_HPP__

#include <deque>
#include <string>

#include "Task.hpp"

using namespace std;

enum TASK_MODE{  GUI,
                 ADD_TASK,
                 LIST_TASK,
                 DELETE_TASK
              };

class Options{

    public:

        /// Default Constructor
        Options();
        
        /// Initialize function
        void init( int argc, char* argv[] );

        void process_args( int argc, char* argv[] );

        /// Set program defaults
        void set_defaults( );
        
        /// Read the configuration file
        void read_config_file( const string& filename );

        /// Write the configuration file
        void write_config_file( const string& filename );
        
        /// Load Tasks
        void load_tasks( string const& task_dir );

        /// List of tasks
        std::deque<Task> tasks;
        
        /// Configuration Filename
        string config_filename;
        
        /// Point Filename
        string point_directory;
        
        /// Remaining Command Line arguments
        std::deque<string> args;

        // Current program mode
        int mode;
};


#endif
