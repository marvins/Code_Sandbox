//
// Created by Marvin Smith on 1/1/18.
//

#ifndef OPENCL_COORDINATE_CONVERTER_COORDINATE_GEOGRAPHIC_HPP
#define OPENCL_COORDINATE_CONVERTER_COORDINATE_GEOGRAPHIC_HPP

// C++ Libraries
#include <cmath>


// Project Libraries
#include "Coordinate_Base.hpp"


/**
 * @class Coordinate_Geographic
 */
class Coordinate_Geographic : public Coordinate_Base
{
    public:

        /**
         * @brief Constructor
         */
        Coordinate_Geographic( const DatumType& datum );


        /**
         * @brief Parameterized Constructor
         */
        Coordinate_Geographic( const double&     longitude_degrees,
                               const double&     latitude_degrees,
                               const DatumType&  datum );


        /**
         * @brief Get the Coordinate Type
         */
        inline CoordinateType Get_Type()const override{
            return CoordinateType::GEOGRAPHIC;
        }


        /**
         * @brief Get the Latitude
         */
        inline double Get_Latitude_Degrees()const{
            return m_latitude_degrees;
        }


        /**
         * @brief Get the Longitude
         */
        inline double Get_Longitude_Degrees()const{
            return m_longitude_degrees;
        }


    private:

        /// Longitude in Degrees
        double m_longitude_degrees;

        /// Latitude in Degrees
        double m_latitude_degrees;

}; // End of Coordinate_Geographic Class

#endif //OPENCL_COORDINATE_CONVERTER_COORDINATE_GEOGRAPHIC_HPP
