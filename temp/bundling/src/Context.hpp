#ifndef __SRC_CONTEXT_HPP__
#define __SRC_CONTEXT_HPP__

#include <deque>

#include "Camera.hpp"
#include "Metrics.hpp"

using namespace std;

class Context{


    public:

        /**
         * Default Constructor.
         *
         * This will load some constant variables.
        */
        Context( const bool& newfile );
        
        /**
         * This will load the current context from a file.
         * If there is a problem, the function will immediately 
         * stop its progress and return false.  This will prompt
         * the program to find the cameras.
        */
        bool load_context( const string& filename );

        /**
         * This will write the current context to a file.
        */
        void write_context( const string& filename )const;
       
        /**
         * Clean out the current context, deleting everything.
        */
        void clear_context();
        
        //this is our camera container
        deque<Camera> cameras;
        
        // this contains all global metrics
        Metrics metrics;

    private:
        
        /**
         * This is the header which we will print to file
        */
        string header;
        
        /**
         * This is the time/date that the file was originally written.
        */
        string initial_timestamp;
        
        /**
         * New file flag
        */
        bool is_new_file;
};


#endif

