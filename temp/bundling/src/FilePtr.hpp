#ifndef __SRC_FILEPTR_HPP__
#define __SRC_FILEPTR_HPP__

#include <deque>
#include <iostream>
#include <string> 

using namespace std;

class FilePtr{
    
    public:

        FilePtr();
        
        int scene_number;
        deque<string> file_parts;

        bool initialized;

};

ostream& operator << ( ostream& ostr, const FilePtr& ptr );

#endif

