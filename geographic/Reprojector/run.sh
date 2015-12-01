#!/bin/sh

SHAPEFILE=data/24k_lakes/24k_lakes-wgs84.shp

#  Application Mode
# - convert : Convert shapefile to desired type
# - project : Render maps
MODE=project

#  Desired Verbosity
#
# -v : debug
#  <blank> : Info
VERBOSITY=-v

#  Mode flags
MODE_FLAGS=


#  Path to shapefile
SHAPEFILE_PATH=data/24k_lakes/24k_lakes.shp


if [ "$MODE" = 'project' ]; then

    #  Process the shapefile flag
    a=4

elif [ "$MODE" = 'convert' ]; then
    
    #  Process the Shapefile flag
    MODE_FLAGS="$MODE_FLAGS -s $SHAPEFILE_PATH"

else
    echo "Unsupported mode flag ($MODE)"
    exit 1
fi


#  Run Projection Application
./reprojector.py -m $MODE $MODE_FLAGS $VERBOSITY

