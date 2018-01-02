/**
 * @file    Coordinate_Base.hpp
 * @author  Marvin Smith
 * @date    1/1/2017
 */
#ifndef COORDINATE_BASE_HPP
#define COORDINATE_BASE_HPP

// C++ Libraries
#include <string>

// Project Libraries
#include "Enumerations.hpp"


/**
 * @class Coordinate_Base
 */
class Coordinate_Base
{
    public:

        /**
         * @brief Constructor
         */
        Coordinate_Base( const DatumType& datum );


    private:

        /// Class Name
        std::string m_class_name;

        /// Datum
        DatumType m_datum;

}; // End of Coordinate_Base Class


#endif