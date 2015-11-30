#!/bin/sh

SHAPEFILE=data/24k_lakes/24k_lakes-wgs84.shp

#  Run Projection Application
./reprojector.py -m convert -s $SHAPEFILE -v
