#include "CoordinateUTM.hpp"

#include "StringUtils.hpp"


using namespace cv;
using namespace std;


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
    datum = _datum;
}


CoordinateUTM::CoordinateUTM( const int& _zone, const bool& _isNorth, const cv::Point2d& coord, const double& _elevation, const std::string& _datum  ){

    zone = _zone;
    isNorth = _isNorth;
    easting = coord.x;
    northing = coord.y;
    elevation = _elevation;
    datum = _datum;
}

CoordinateUTM::CoordinateUTM( const int& _zone, const bool& _isNorth, const cv::Point3d& point, const string& _datum ){
    
    zone = _zone;
    isNorth = _isNorth;
    easting = point.x;
    northing = point.y;
    elevation = point.z;
    datum = _datum;
}

Point2d CoordinateUTM::toPoint2d( )const{
    return Point2d( easting, northing );
}

Point3d CoordinateUTM::toPoint3d( )const{
    return Point3d( easting, northing, elevation );
}

CoordinateUTM CoordinateUTM::operator + ( CoordinateUTM const& rhs )const{

    // throw an exception if the regions are different
    if( zone != rhs.zone || isNorth != rhs.isNorth )
        throw string("ERROR: Zones are different, exiting");

    // compute the sum
    return CoordinateUTM( zone, isNorth, easting + rhs.easting, northing + rhs.northing, elevation + rhs.elevation, datum );

}

/**
 * Compute the distance between two vectors
*/
CoordinateUTM CoordinateUTM::operator - ( CoordinateUTM const& rhs )const{

    //  throw an exception if the regions are different
    if( zone != rhs.zone || isNorth != rhs.isNorth )
        throw string("ERROR: Zones are different, exiting");
    
    // compute the difference
    return CoordinateUTM( zone, isNorth, easting - rhs.easting, northing - rhs.northing, elevation - rhs.elevation, datum );

}

double CoordinateUTM::mag2()const{
    return (easting*easting + northing*northing + elevation*elevation);
}

double CoordinateUTM::mag()const{
    return sqrt(mag2());
}

string CoordinateUTM::toString()const{

    if( isNorth == true )
        return (string("<") + num2str(zone) + string(", Hemi:N, E:") + num2str(easting) + string(", N:") + num2str(northing) + string(", Elev: ") + num2str(elevation) + string(", D:") + datum +  string(">"));
    else    
        return (string("<") + num2str(zone) + string(", Hemi:S, E:") + num2str(easting) + string(", N:") + num2str(northing) + string(", Elev: ") + num2str(elevation) + string(", D:") + datum +  string(">"));
}


CoordinateUTM operator * ( CoordinateUTM const& pnt, double const& s ){
    return CoordinateUTM( pnt.zone, pnt.isNorth, pnt.easting*s, pnt.northing*s, pnt.elevation*s );

}

CoordinateUTM operator * ( double const& s, CoordinateUTM const& pnt ){
    return CoordinateUTM( pnt.zone, pnt.isNorth, pnt.easting*s, pnt.northing*s, pnt.elevation*s );
}

double dot( CoordinateUTM const& PA, CoordinateUTM const& PB ){
    
    return ((PA.easting * PB.easting)+(PA.northing * PB.northing)+(PA.elevation * PB.elevation));
}

