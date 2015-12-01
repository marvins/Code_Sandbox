#!/bin/sh

#  Application Mode
# - project : Render maps
MODE=project

#  Desired Verbosity
#
# -v : debug
#  <blank> : Info
VERBOSITY=-v

#  Mode flags
MODE_FLAGS=

#CONFIG_PATH=data/options-states.cfg
#CONFIG_PATH=data/options-nevada.cfg
CONFIG_PATH=data/options-globe.cfg

#  List of Projections
#PROJECTIONS="geographic_direct utm cassini bonne armadillo"
#PROJECTIONS="geographic_direct"
PROJECTIONS="armadillo"

if [ "$MODE" = 'project' ]; then
    
    #  Set the Config
    echo '' &> /dev/null

else
    echo "Unsupported mode flag ($MODE)"
    exit 1
fi


#  Run Projection Application
for PROJECTION in $PROJECTIONS; do
    echo "Running with $PROJECTION projection."
    ./reprojector.py -m $MODE $MODE_FLAGS -c $CONFIG_PATH -p $PROJECTION $VERBOSITY
done

