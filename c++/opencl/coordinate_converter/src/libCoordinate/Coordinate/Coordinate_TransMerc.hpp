/**
 * @file    CoordinateTransMerc.hpp
 * @author  Marvin Smith
 * @date    1/13/2017
 * @brief   Coordinate Structure for Transverse Mercator Projections
 */
#ifndef LIB_COORDINATE_COORDINATE_TRANSMERC_HPP
#define LIB_COORDINATE_COORDINATE_TRANSMERC_HPP

// Project Libraries
#include "Coordinate_Base.hpp"

class Coordinate_TransMerc : public Coordinate_Base
{
    public:

        /**
         * @brief Constructor
         */
        Coordinate_TransMerc( const DatumType& datum );


        /**
         * @brief Constructor
         *
         * @param[in] grid_zone
         * @param[in] is_northern
         * @param[in] easting
         * @param[in] northing
         * @param[in] datum
         */
        Coordinate_TransMerc( const double&     easting,
                              const double&     northing,
                              const DatumType&  datum );


        /**
         * @brief Get the Coordinate Type
         */
        inline CoordinateType Get_Type()const override{
            return CoordinateType::TRANS_MERC;
        }


        /**
         * @brief Get the Easting in Meters
         */
        inline double Get_Easting_Meters()const{
            return m_easting;
        }


        /**
         * @brief Get the Northing in Meters
         * @return
         */
        inline double Get_Northing_Meters()const{
            return m_northing;
        }

    private:

        /// Easting
        double m_easting;

        /// Northing
        double m_northing;

}; // End of Coordinate_TransMerc Class

#endif //OPENCL_COORDINATE_CONVERTER_COORDINATE_UTM_HPP
