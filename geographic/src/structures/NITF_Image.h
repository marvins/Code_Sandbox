#ifndef __SRC_NITF_HEADER_H__
#define __SRC_NITF_HEADER_H__

#include <string>

using namespace std;

class NITF_Image{

    public:

        NITF_Image();
        NITF_Image(const string& fname, const bool& init = false );

        void set_filename( const string& fname );
        string get_filename( )const;
    
    private:

        void load_image();

        string filename;
        bool initialize;
};

#endif
