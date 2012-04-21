#ifndef __GEO_HEADER_H__
#define __GEO_HEADER_H__

#include <string>

class GeoHeader_Info{

   public:
      GeoHeader_Info();

      virtual ~GeoHeader_Info();

      std::string get_image_filename()const;

      void set_image_filename( std::string const& filename );

      virtual void copy_header_info(GeoHeader_Info* other ) = 0;

      bool image_filename_exists()const;

   protected:

      std::string image_filename;


};


#endif
