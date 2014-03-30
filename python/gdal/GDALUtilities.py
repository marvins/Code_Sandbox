#    File:    GDALUtilities.py
#    Author:  Marvin Smith
#    Date:    3/25/2014
#
#    Purpose: Provide helper functions requiring GDAL
#

#  Import GDAL
import osr

#  Convert a Coordinate From UTM to Lat Lon
def convertUTM2LatLon( zone, easting, northing, altitude = 0, CoordinateSystem = 'WGS84'):
	
	#  Create the UTM Coordinate System
	utm_cs = osr.SpatialReference();
	utm_cs.SetWellKnownGeogCS(CoordinateSystem);
	
	#  Determine if we are in the northern hemisphere
	is_northern = float(northing) > 0

	# Compute UTM Zone
	utm_cs.SetUTM(int(zone), is_northern)

	#  Create output Coordinate System
	wgs84_cs = utm_cs.CloneGeogCS();

	#  Create the transform
	utm2wgs84_transform = osr.CoordinateTransformation(utm_cs, wgs84_cs)

	#  Transform the point
	latLonCoord = utm2wgs84_transform.TransformPoint(float(easting),float(northing),float(altitude));

	return [latLonCoord[1],latLonCoord[0],latLonCoord[2]];


#  Convert a Coordinate from Lat Lon to UTM
def convertLatLon2UTM( latitude, longitude, altitude, CoordinateSystem = 'WGS84', setZone = -1 ):
	
	def get_utm_zone(longitude):
		return (int(1+(longitude+180.0)/6.0));
	
	def is_northern(latitude):
		if( latitude < 0.0 ):
			return 0;
		else:
			return 1;

	#  create utm coordinate system
	utm_cs = osr.SpatialReference();

	#  Set the CS
	utm_cs.SetWellKnownGeogCS(CoordinateSystem);

	zone = get_utm_zone(longitude);
	utm_cs.SetUTM( zone, is_northern(latitude));
	
	wgs84_cs = utm_cs.CloneGeogCS();

	#  Create transform component
	wgs2utm_transform = osr.CoordinateTransformation( wgs84_cs, utm_cs);

	utmCoord = wgs2utm_transform.TransformPoint(longitude, latitude, altitude);
	return [get_utm_zone(longitude), utmCoord[0], utmCoord[1], utmCoord[2]];


