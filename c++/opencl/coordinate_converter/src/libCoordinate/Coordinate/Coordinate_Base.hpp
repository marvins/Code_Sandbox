/**
 * @file    Coordinate_Base.hpp
 * @author  Marvin Smith
 * @date    1/1/2017
 */
#ifndef COORDINATE_BASE_HPP
#define COORDINATE_BASE_HPP

// C++ Libraries
#include <memory>
#include <string>

// Project Libraries
#include "Enumerations.hpp"


/**
 * @class Coordinate_Base
 */
class Coordinate_Base
{
    public:

        /// Pointer Type
        typedef std::shared_ptr<Coordinate_Base> ptr_t;


        /**
         * @brief Constructor
         */
        Coordinate_Base( const DatumType& datum = DatumType::UNKNOWN );


        /**
         * @brief Get the Datum
         */
        inline DatumType Get_Datum()const{
            return m_datum;
        }


        /**
         * @brief Get the Coordinate Type
         */
        virtual CoordinateType Get_Type()const = 0;


    private:

        /// Class Name
        std::string m_class_name;

        /// Datum
        DatumType m_datum;

}; // End of Coordinate_Base Class


#endif