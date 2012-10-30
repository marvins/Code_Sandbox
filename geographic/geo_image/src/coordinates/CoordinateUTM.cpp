#include "CoordinateUTM.hpp"

#include "../utilities/String_Utilities.hpp"

using namespace std;

namespace GEO{

CoordinateUTM::CoordinateUTM(){
    zone = 0;
    hemisphere = 'N';
    easting = 166022;
    northing = 0;
    
    datum = WGS84;
}


CoordinateUTM::CoordinateUTM( const int& _zone, const char& _hemi, const double& _easting, const double& _northing ){

    zone = _zone;
    hemisphere = _hemi;
    easting = _easting;
    northing = _northing;

    datum = WGS84;
}


string CoordinateUTM::toString()const{

    return (string("<") + STR::num2str(zone) + string(", E:") + STR::num2str(easting) + string(", N:") + STR::num2str(northing) + string(">"));
}

}
