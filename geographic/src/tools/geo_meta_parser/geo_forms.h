#ifndef __SRC_TOOLS_GEO_META_PARSER_GEO_FORMS_H__
#define __SRC_TOOLS_GEO_META_PARSER_GEO_FORMS_H__

#include <string>
#include <vector>

enum  meta_format{
        
        META_INFORMATIONAL  = 0,
        META_GAP            = -1,
        META_GENERIC        = 1,
        META_COORDINATE     = 2,
        META_IMAGE_TYPE     = 3,
        META_IMAGE_FILENAME = 4,
};

class geo_header_item{
    public:
        
        geo_header_item();
        geo_header_item( const int& idx, const std::string& type, const std::string& val );

        /** Header Types
            1. Generic Metadata  (string)
            2. Geographic Corner (double)
            3. Image Type        (int)
            4. Image Filename    (string)
        */
        int         header_type;

        std::string header_tag;
        std::string header_val;

};

void print_header( std::string const& input_message, const int& start_y, const int& width, const int& height );

void print_metadata( std::vector<geo_header_item> const& header_metadata, const int& start_y, const int&  con_size_x, const int& con_size_y, const int& cursor_pos, int& window_top  );

void print_footer( const int& con_size_x, const int& con_size_y );

bool change_screen( geo_header_item& item, const int& width, const int& height  );

int save_screen( const int& width, const int& height );

#endif

