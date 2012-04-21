#ifndef __GEO_HEADER_H__
#define __GEO_HEADER_H__

#include <string>

class GeoHeader_Info{

    GeoHeader_Info();

    virtual ~GeoHeader_Info();

    std::string get_image_filename()const;

    void set_image_filename( std::string const& filename );
    
    private:

        std::string image_filename;


}


#endif
