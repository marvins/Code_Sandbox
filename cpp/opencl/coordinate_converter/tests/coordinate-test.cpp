/**
 * @file    coordinate-test.cpp
 * @author  Marvin Smith
 * @date    1/13/2017
 */
#include <gtest/gtest.h>

// Project Libraries
#include "Coordinate_Test_Data.hpp"


int main( int argc, char* argv[] )
{
    // Create the Coordinate Test Harness
    Coordinate_Test_Data::Initialize( argv[1]);

    // Initialize the Unit-Tests
    ::testing::InitGoogleTest(&argc, argv);
    auto exit_code = RUN_ALL_TESTS();

    // Clean up the harness
    Coordinate_Test_Data::Finalize();

    return exit_code;
}