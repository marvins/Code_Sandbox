#include "TEST_coordinate.hpp"

// add Logging code
#include <Logger.hpp>

// add GeoImage library
#include <GeoImage.hpp>

#include <iostream>

using namespace std;

int TEST_coordinate_constructor( string& note );
int TEST_coordinate_conversions( string& note );

void TEST_coordinate_module(){

    /** test image data */
    int result;
    string note;

    print_module_header("Coordinate");
    
    // test coordinate constructors
    result = TEST_coordinate_constructor( note );
    print_test_results( "Coordinate    Constructor", result, note );

    // test coordinate conversions
    result = TEST_coordinate_conversions( note );
    print_test_results( "Coordinate    Conversions", result, note );


    print_module_footer("Coordinate");

}


int TEST_coordinate_constructor( string& note ){
    
    // create some coordinates
    GEO::CoordinateLatLon c01( 36.578581, -118.291995 );
    GEO::CoordinateLatLon c02( 36, 34, 42.89, -118, 17, 31.18 );
    GEO::CoordinateUTM    c03( 11, 'S', 384409, 4048901 );

    note = "Constructor failed";
    if( fabs( c01.lat -  36.578581 ) > 0.0001 )  return false;
    if( fabs( c02.lat -  36.578581 ) > 0.0001 )  return false;
    if( fabs( c01.lon + 118.291995 ) > 0.0001 )  return false;
    if( fabs( c02.lon + 118.291995 ) > 0.0001 )  return false;
    if( c03.zone != 11 || c03.hemisphere != 'S' || fabs( c03.easting - 384409 ) > 0.00001 || fabs( c03.northing- 4048901) > 0.00001 ) return false;

    // exit
    note = "Operation Sucessful";
    return 1;
}

int TEST_coordinate_conversions( string& note ){


    // create some coordinates
    GEO::CoordinateLatLon c01( 36.578581, -118.291995 );
    GEO::CoordinateLatLon c02( 36, 34, 42.89, -118, 17, 31.18 );
    GEO::CoordinateUTM    c03( 11, 'N', 384409, 4048901 );
    
    // Convert LatLon to UTM
    GEO::CoordinateLatLon  result03 = GEO::convert_coordinate( c03 );
    

    note = "Operation Successful";
    return 1;
}


