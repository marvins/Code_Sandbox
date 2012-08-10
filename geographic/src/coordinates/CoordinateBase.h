#ifndef __SRC_COORDINATE_COORDINATEBASE_H__
#define __SRC_COORDINATE_COORDINATEBASE_H__

#include <string>

namespace GEO{
    
    enum DATUM_TYPES{
        NAD83 = 0,
        WGS72 = 1,
        WGS84 = 2
    };


    /**
     * @class CoordinateBase
    */
    class CoordinateBase{

        public:
        
            /**
             * To String Function
             *
             * @return String output
            */
            virtual std::string toString()const = 0;

    };
    

    std::string datum2string( const int& datum );
}

#endif
