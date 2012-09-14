#! /usr/bin/env bash

# This is the location of the Medusa repository
MEDUSA_PATH=/home/ms6401/projects/Medusa

GEOIMAGE_HOME=geographic
GEOIMAGE_DEST=GeoImage


# Copy over geoimage code
cp -r ${GEOIMAGE} ${MEDUSA_PATH}/${GEOIMAGE_DEST}

