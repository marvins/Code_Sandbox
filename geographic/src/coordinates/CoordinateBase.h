#ifndef __SRC_COORDINATE_COORDINATEBASE_H__
#define __SRC_COORDINATE_COORDINATEBASE_H__

#include <string>

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
        std::string toString()const = 0;

};

#endif
