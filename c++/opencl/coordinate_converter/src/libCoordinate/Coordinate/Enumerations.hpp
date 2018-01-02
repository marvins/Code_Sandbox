//
// Created by Marvin Smith on 1/1/18.
//

#ifndef OPENCL_COORDINATE_CONVERTER_ENUMERATIONS_HPP
#define OPENCL_COORDINATE_CONVERTER_ENUMERATIONS_HPP

// C++ Libraries
#include <cinttypes>
#include <string>


enum class DatumType : int32_t
{
    UNKNOWN = 0 /**< Error Condition. */,
    WGS84   = 1 /**< WGS 84 Ellipsoid */,
};

#endif //OPENCL_COORDINATE_CONVERTER_ENUMERATIONS_HPP
