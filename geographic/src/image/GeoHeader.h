#ifndef __GEO_HEADER_H__
#define __GEO_HEADER_H__

#include <string>
#include <utility>
#include <vector>

#include "PixelType.h"

namespace GEO{

/**
 * @class Base Header Class. 
 */
class GeoHeader_Info{

    public:

        /** 
         * Default constructor for GeoHeader
         */
        GeoHeader_Info();

        /**
         * Destructor for GeoHeader
         */
        virtual ~GeoHeader_Info();

        /**
         * Retrieve the image filename.
         *
         * @brief Retrieve the image filename.
         *
         * @return filename used to load image
         */
        std::string get_image_filename()const;

        /**
         * Set the image filename.
         *
         * @brief Set the image filename. 
         *
         * @param[in] desired image filename.
         */
        void set_image_filename( std::string const& filename );

        /**
         * Copy the header information. Performs a deep copy.
         *
         * @brief Copy the header data. 
         */
        virtual void copy_header_info(GeoHeader_Info* other ) = 0;

        /**
         * Check if an image file exists.
         *
         * @return true if the file exists.
         */
        bool image_filename_exists()const;

        /** 
         * Static function which will verify if an input filename exists
         * 
         * @param[in] fname 
         * @return    true if file exists, false otherwise
        */
        static bool file_exists( std::string const& filename );

        /** 
         * Set the pixel type of the image.  The PixelType 
         * object must be one of the specified types in 
         * PixelType.h.
         *
         * @brief Set the pixel type of the image.
         *
         * @param[in] pix Desired PixelType
         */
        void set_pixel_type( PixelType const& pix);

        /**
         * Retrieve the PixelType of the image.
         *
         * @brief Retrieve the PixelType of the image.
         *
         * @return Specified PixelType
         */
        PixelType get_pixel_type()const;

        /** 
         * Clone the header using a deep copy. 
         *
         * @brief Clone the header using a deep copy.
         *
         * @return Clone of GeoHeader object
         */
        virtual GeoHeader_Info*& clone()const = 0;

        /** 
         * Retrieve the driver format.
         *
         * @brief Retrieve the driver format.
         *
         * @return Driver Format as String
         */
        virtual std::string get_driver_format()const = 0;
        virtual std::string get_gdal_driver_format()const = 0;

        /** 
         * Check if the data is valid on the header.
         *
         * @brief Check if the data is valid on the header.
         *
         * @return The State of the Header Object
         */
        bool isValid()const;

        /**  
         * Set the validity flag of the header.  This is not recommended for 
         * the user to set unless they are creating their own header from scratch. 
         *
         * @brief Set the validity flag of the header object.
         *
         * @param[in] flag flag value to set
         */
        void setValid( const bool& flag );

        void set_header_data( std::vector<std::pair<std::string,std::string> >const& hdata ); 
        std::vector<std::pair<std::string,std::string> > get_header_data()const;

        bool get_header_item( std::string const& key, std::string& result );
        bool set_header_item( std::string const& key, std::string const& value );

    protected:

        PixelType pixeltype;            /*<  Pixeltype of GDAL Data */
        std::string image_filename;     /*<  Image filename */
        bool valid; 		            /*<  Validity flag signifying if data is loaded */
        std::vector<std::pair<std::string,std::string> >  header_data;  /*<  */

};

} //end of GEO namespace 

#endif
