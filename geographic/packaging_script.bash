#! /usr/bin/env bash

# This is the location of the Medusa repository
MEDUSA_PATH=/home/ms6401/projects/Medusa

GEOIMAGE_SRC=src
GEOIMAGE_DEST=GeoImage

# Make sure destination directory exists
if [ ! -d "${MEDUSA_PATH}/${GEOIMAGE_DEST}" ]; then 
    mkdir ${MEDUSA_PATH}/${GEOIMAGE_DEST}
fi

# Copy over geoimage code
cp -r ${GEOIMAGE_SRC} ${MEDUSA_PATH}/${GEOIMAGE_DEST}/${GEOIMAGE_SRC}

