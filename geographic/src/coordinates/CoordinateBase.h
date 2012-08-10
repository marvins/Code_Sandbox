#ifndef __SRC_COORDINATE_COORDINATEBASE_H__
#define __SRC_COORDINATE_COORDINATEBASE_H__

#include <string>

namespace GEO{

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

}

#endif
