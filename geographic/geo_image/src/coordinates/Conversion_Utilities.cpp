#include "Conversion_Utilities.hpp"

#include "Datum.hpp"

#include <iostream>
#include <string>

#include <ogr_spatialref.h>

using namespace std;

namespace GEO{

/**
 * Compute the best utm zone.
*/
int compute_utm_zone( CoordinateLatLon const& coord, bool& isNorth ){

    // check the isNorth flag
    if( coord.lat >= 0 ) isNorth = true;
    else                 isNorth = false;

    // compute the appropriate UTM Zone
    int zone = std::floor(( coord.lon + 180 )/6.0) + 1;
    
    return zone;
}

CoordinateLatLon convert_coordinate( CoordinateUTM const& coord ){
    
    // create a spatial reference object
    OGRSpatialReference sourceSRS, targetSRS;

    //set it to the required DATUM
    sourceSRS.SetWellKnownGeogCS( coord.datum.c_str() );
    targetSRS.SetWellKnownGeogCS( coord.datum.c_str() );

    // set the UTM Components
    sourceSRS.SetUTM( coord.zone, coord.isNorth );
    
    // Create the Coordinate Transformation
    OGRCoordinateTransformation *poTransform = OGRCreateCoordinateTransformation( &sourceSRS, &targetSRS );
    if( poTransform == NULL ){
        throw string("ERROR: call failed");
    }

    double x = coord.easting;
    double y = coord.northing;
    double z = coord.elevation;
    
    if( !poTransform->Transform( 1, &x, &y, &z ) )
        throw string("ERROR: transform failed");
   
    OCTDestroyCoordinateTransformation( poTransform );

    //create output coordinate
    return CoordinateLatLon( y, x, z, coord.datum );
}


CoordinateUTM convert_coordinate( CoordinateLatLon const& coord ){

    // create the spatial reference objects
    OGRSpatialReference sourceSRS, targetSRS;
    
    // set the required datim
    sourceSRS.SetWellKnownGeogCS( coord.datum.c_str() );
    targetSRS.SetWellKnownGeogCS( coord.datum.c_str() );
    
    // specify the target as UTM and compute proper zone and hemisphere
    bool isNorth;
    int zone = compute_utm_zone( coord, isNorth );
    targetSRS.SetUTM( zone, isNorth );

    // create the coordinate transform
    OGRCoordinateTransformation *poTransform = OGRCreateCoordinateTransformation( &sourceSRS, &targetSRS );
    if( poTransform == NULL ){
        throw string("ERROR: call failed");
    }
   
    
    double x = coord.lon;
    double y = coord.lat;
    double z = coord.elevation;
    
    if( !poTransform->Transform( 1, &x, &y, &z ) )
        throw string("ERROR: transform failed");
    
    OCTDestroyCoordinateTransformation( poTransform );
    
    return CoordinateUTM( zone, isNorth, x, y, z, coord.datum );

}


} // End of GEO Namespace 

