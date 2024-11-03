/**
 * @file    Ellipsoid.hpp
 * @author  Marvin Smith
 * @date    1/21/2017
 */
#ifndef LIB_COORDINATE_ELLIPSOID_HPP
#define LIB_COORDINATE_ELLIPSOID_HPP

#include "Enumerations.hpp"

/**
 * @class Ellipsoid
 */
class Ellipsoid
{
    public:

        /**
         * @brief Default Constructor
         */
        Ellipsoid();


        /**
         * @brief Constructor.
         *
         * @param[in] datum_id ID of the datum to use.
         * @param[in] equatorial_radius Equatorial radius of the ellipse in meters.
         * @param[in] eccentricity_squared Eccentricity squared of the ellipse.
         */
        Ellipsoid( const DatumType&  datum_id,
                   double const&     equatorial_radius,
                   double const&     eccentricity_squared );


        /**
          * @brief Get the Equatorial Radius.
          *
          * @return Equitorial radius in meters.
          */
        inline double Get_Equatorial_Radius()const{
            return m_equatorial_radius;
        }


        /**
         * @brief Get the Eccentricity Squared.
         *
         * @return Eccentricity squared value.
         */
        inline double Get_Eccentricity_Squared()const{
            return m_eccentricity_squared;
        }


    private:

        /// Elllipsoid Datum
        DatumType m_datum_type;

        /// Equatorial Radius
        double m_equatorial_radius;

        /// Eccentricity Squared
        double m_eccentricity_squared;

};


/**
 * @brief Get the Requested Ellipsoid
 * @param ellipsoid
 * @return
 */
Ellipsoid  Get_Ellipsoid( const DatumType& ellipsoid,
                          bool&            found );


#endif