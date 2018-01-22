/**
 * @file    TEST_libConvert01.cpp
 * @author  Marvin Smith
 * @date    1/13/2017
 */
#include <gtest/gtest.h>

// Project Libraries
#include <OpenCL_Convert_01.hpp>
#include <Coordinate.hpp>

// Unit-Test Libraries
#include "Coordinate_Test_Data.hpp"


/***********************************************************/
/*          Test the Geographic to UTM Conversion          */
/***********************************************************/
TEST( OpenCL_Convert_01, Convert_Geographic_WGS84_To_UTM )
{
    // Get the Reference Coordinate List
    auto geog_coords = Coordinate_Test_Data::Get_Coordinates( CoordinateType::GEOGRAPHIC, DatumType::WGS84 );
    auto utm_coords  = Coordinate_Test_Data::Get_Coordinates( CoordinateType::UTM,        DatumType::WGS84 );

    ASSERT_EQ( geog_coords.size(), 2 );
    ASSERT_EQ( utm_coords.size(), 2 );

    //  Convert the Coordinates to a Vector
    std::vector<Coordinate_Geographic> geog_list;
    for( auto geog_coord : geog_coords )
    {
        Coordinate_Geographic gc = *std::dynamic_pointer_cast<Coordinate_Geographic>(geog_coord.second);
        geog_list.push_back(gc);
    }

    // Create Converter
    auto converter_config = std::make_shared<OpenCL_Converter_Config_01>();
    OpenCL_Converter_01  converter( converter_config );

    // Batch Convert to UTM
    auto utm_list = converter.Convert_To_UTM( geog_list );

    // Check the output size
    ASSERT_EQ( utm_list.size(), geog_list.size());

    // Compare Contents
    int cnt = 0;
    for( auto utm_crd : utm_coords )
    {
        // Cast the reference coordinate
        auto ref_utm_crd = std::dynamic_pointer_cast<Coordinate_UTM>(utm_crd.second);

        // Compare Grid Zone
        ASSERT_EQ( ref_utm_crd->Get_Grid_Zone(),
                   utm_list[cnt].Get_Grid_Zone() );

        ASSERT_EQ( ref_utm_crd->Is_Northern(),
                   ref_utm_crd->Is_Northern() );

        // Compare Easting
        ASSERT_NEAR( ref_utm_crd->Get_Easting_Meters(),
                     utm_list[cnt].Get_Easting_Meters(), 1 );

        // Compare Northing
        ASSERT_NEAR( ref_utm_crd->Get_Northing_Meters(),
                    utm_list[cnt].Get_Northing_Meters(), 1 );

        // Increment Counter
        cnt++;
    }
}