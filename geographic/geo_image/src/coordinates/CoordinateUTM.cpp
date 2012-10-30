#include "CoordinateUTM.hpp"

#include "../utilities/String_Utilities.hpp"

using namespace std;

namespace GEO{

CoordinateUTM::CoordinateUTM(){
    
    zone = 0;
    isNorth = true;
    easting = 166022;
    northing = 0;
    elevation = 0;
    
    datum = "WGS84";
}


CoordinateUTM::CoordinateUTM( const int& _zone, const bool& _isNorth, const double& _easting, const double& _northing, const double& _elevation, const string& _datum ){

    zone = _zone;
    isNorth = _isNorth;
    easting = _easting;
    northing = _northing;

    elevation = _elevation;
    datum = "WGS84";
}


string CoordinateUTM::toString()const{

    if( isNorth == true )
        return (string("<") + STR::num2str(zone) + string(", Hemi:N, E:") + STR::num2str(easting) + string(", N:") + STR::num2str(northing) + string(", Elev: ") + STR::num2str(elevation) + string(", D:") + datum +  string(">"));
    else    
        return (string("<") + STR::num2str(zone) + string(", Hemi:S, E:") + STR::num2str(easting) + string(", N:") + STR::num2str(northing) + string(", Elev: ") + STR::num2str(elevation) + string(", D:") + datum +  string(">"));
}

}
