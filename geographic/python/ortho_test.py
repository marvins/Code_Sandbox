#! /usr/bin/env python

class UTMPoint:
	zone       = 0
	hemisphere = 0
	easting    = 0
	northing   = 0

class GeodeticPoint:
	
	latitude  = 0
	longitude = 0

print 'Orthorectification Test'

# Create a fake camera system
focal_length = 0.20 # Meters
H  = 4800 # Meters


# Compute the Nadir Point

