//
// Created by Marvin Smith on 1/1/18.
//

#ifndef OPENCL_COORDINATE_CONVERTER_COORDINATE_UTM_HPP
#define OPENCL_COORDINATE_CONVERTER_COORDINATE_UTM_HPP

// Project Libraries
#include "Coordinate_TransMerc.hpp"


/**
 * @class Coordinate_UTM
 */
class Coordinate_UTM : public Coordinate_TransMerc
{
    public:

        /**
         * @brief Constructor
         */
        Coordinate_UTM();


        /**
         * @brief Constructor
         *
         * @param[in] grid_zone
         * @param[in] is_northern
         * @param[in] easting
         * @param[in] northing
         * @param[in] datum
         */
        Coordinate_UTM( const int32_t&    grid_zone,
                        const bool&       is_northern,
                        const double&     easting,
                        const double&     northing );


        /**
         * @brief Get the Coordinate Type
         */
        inline CoordinateType Get_Type()const override{
            return CoordinateType::UTM;
        }


        /**
         * @brief Get the Grid Zone
         * @return
         */
        inline int32_t Get_Grid_Zone()const{
            return m_grid_zone;
        }


        /**
         * @brief Northern Hemisphere
         * @return
         */
        inline bool Is_Northern()const{
            return m_is_northern;
        }

    private:

        /// Grid Zone
        int32_t m_grid_zone;

        /// Hemisphere
        bool m_is_northern;

}; // End of Coordinate_UTM Class

#endif //OPENCL_COORDINATE_CONVERTER_COORDINATE_UTM_HPP
