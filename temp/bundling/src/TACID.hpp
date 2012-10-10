#ifndef __SRC_TACID_HPP__
#define __SRC_TACID_HPP__

#include <string>

using namespace std;

class TACID{

    public:
        
        /**
         * Default Constructor
        */
        TACID( ){


        }
        

        static int scene_number( string const& filename, bool& isValid );

    private:

        string raw_string;

};


#endif

