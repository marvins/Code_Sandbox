#ifndef __SRC_CORE_TASK_HPP__
#define __SRC_CORE_TASK_HPP__

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Task{

    public:
        
        Task();

        Task( const string& taskname );

        void read( const string& filename );

        /// Name
        string name;

        /// Filename
        string task_filename;
        
        /// Groups
        vector<string> groups;
        
        /// Note
        vector<string> notes;

};

ostream& operator << ( ostream& ostr, const Task& task );

#endif
