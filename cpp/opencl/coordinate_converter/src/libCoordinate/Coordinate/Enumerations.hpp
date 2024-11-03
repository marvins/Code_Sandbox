//
// Created by Marvin Smith on 1/1/18.
//

#ifndef OPENCL_COORDINATE_CONVERTER_ENUMERATIONS_HPP
#define OPENCL_COORDINATE_CONVERTER_ENUMERATIONS_HPP

// C++ Libraries
#include <cinttypes>
#include <string>


/**
 * @enum DatumType
 */
enum class DatumType : int32_t
{
    UNKNOWN = 0 /**< Error Condition. */,
    WGS84   = 1 /**< WGS 84 Ellipsoid */,
};


/**
 * @brief Convert a DatumType to a String
 * @param datum_type
 * @return
 */
std::string DatumType_To_String( const DatumType&  datum_type );


/**
 * @brief Convert a String to DatumType Enumeration
 * @param datum_type
 * @return
 */
DatumType  String_To_DatumType( const std::string& datum_type );


/**
 * @enum CoordinateType
 */
enum class CoordinateType : int32_t
{
    UNKNOWN    = 0 /**< Error Condition.*/,
    GEOGRAPHIC = 1 /**< Standard Spherical Coordinate System.*/,
    MERCATOR   = 2 /**< Mercator Projection */,
    TRANS_MERC = 3 /**< Transverse Mercator */,
    UTM        = 4 /**< Universal Transverse Mercator */,
};


/**
 * @brief Convert a CoordinateType to a String
 * @param coordinate_type
 * @return
 */
std::string  CoordinateType_To_String( const std::string& coordinate_type );


/**
 * @brief Convert a String into a CoordinateType enumn
 * @param coordinate_type
 * @return
 */
CoordinateType   String_To_CoordinateType( const std::string& coordinate_type );


#endif //OPENCL_COORDINATE_CONVERTER_ENUMERATIONS_HPP
