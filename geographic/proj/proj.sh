#!/bin/sh

#  Define Latitude and Longitude
LAT='39.534156'
LON='-119.702771'

#  Define the desired output UTM Zone
OUTPUT_ZONE=11

#  Perform Conversion
echo $LON $LAT | cs2cs +proj=longlat +datum=WGS84 +to +proj=utm +zone=$OUTPUT_ZONE +datum=WGS84


