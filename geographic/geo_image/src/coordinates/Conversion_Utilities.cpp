#include "Conversion_Utilities.hpp"

#include "Datum.hpp"

#include <iostream>
#include <string>

#include <ogr_spatialref.h>

using namespace std;

namespace GEO{

CoordinateLatLon convert_coordinate( CoordinateUTM const& coord ){
    
    // create a spatial reference object
    OGRSpatialReference sourceSRS, targetSRS;

    //set it to the required DATUM
    sourceSRS.SetWellKnownGeogCS( datum2string(coord.datum).c_str() );
    targetSRS.SetWellKnownGeogCS( datum2string(coord.datum).c_str() );

    // set the UTM Components
    if( coord.hemisphere == 'N' )
        sourceSRS.SetUTM( coord.zone, true );
    else
        sourceSRS.SetUTM( coord.zone, false );
    
    // Create the Coordinate Transformation
    OGRCoordinateTransformation *poTransform = OGRCreateCoordinateTransformation( &sourceSRS, &targetSRS );
    if( poTransform == NULL ){
        throw string("ERROR: call failed");
    }

    double x = coord.easting;
    double y = coord.northing;
    
    if( !poTransform->Transform( 1, &x, &y ) )
        throw string("ERROR: transform failed");

    //create output coordinate
    return CoordinateLatLon( y, x, coord.datum );

}


}

