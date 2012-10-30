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
    GEO::CoordinateUTM    c03( 11, true, 384409, 4048901 );

    note = "Constructor failed";
    if( fabs( c01.lat -  36.578581 ) > 0.0001 )  return false;
    if( fabs( c02.lat -  36.578581 ) > 0.0001 )  return false;
    if( fabs( c01.lon + 118.291995 ) > 0.0001 )  return false;
    if( fabs( c02.lon + 118.291995 ) > 0.0001 )  return false;
    if( c03.zone != 11 || c03.isNorth != true || fabs( c03.easting - 384409 ) > 0.00001 || fabs( c03.northing- 4048901) > 0.00001 ) return false;

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
    GEO::CoordinateLatLon  result01 = GEO::convert_coordinate( c03 );
    GEO::CoordinateUTM     result02 = GEO::convert_coordinate( c02 );
    
    note = "Conversion Failed";
    if( fabs( result01.lat       - c01.lat       ) > 0.001 ||
        fabs( result01.lon       - c01.lon       ) > 0.001 ||
        fabs( result01.elevation - c01.elevation ) > 0.001 || 
        result01.datum != c01.datum                          ) return false;
    
    if( fabs( result02.easting   - c03.easting   ) > 1 ||
        fabs( result02.northing  - c03.northing  ) > 1 ||
        fabs( result02.elevation - c03.elevation ) > 1 ||
        result02.datum          != c03.datum     )            return false;
    
    // test conversion accuracy
    GEO::CoordinateLatLon result10 = GEO::convert_coordinate( GEO::convert_coordinate( c01 ));
    GEO::CoordinateLatLon result11 = GEO::convert_coordinate( GEO::convert_coordinate( c02 ));
    GEO::CoordinateUTM    result12 = GEO::convert_coordinate( GEO::convert_coordinate( c03 ));
    
    note = "Reprojection Failed";
    if( fabs( result10.lat - c01.lat ) > 0.001 || fabs( result10.lon - c01.lon ) > 0.001 || fabs( result10.elevation - c01.elevation ) > 0.001 ) return false;
    if( fabs( result11.lat - c02.lat ) > 0.001 || fabs( result11.lon - c02.lon ) > 0.001 || fabs( result11.elevation - c02.elevation ) > 0.001 ) return false;
    if( fabs( result12.easting - c03.easting ) > 0.001 || fabs( result12.northing - c03.northing ) > 0.001 || fabs( result12.elevation - c03.elevation ) > 0.001 ) return false;

    note = "Operation Successful";
    return 1;
}


