#! /usr/bin/env bash

#  This is a script which will transfer the code currently stored in this
#  repository into the Medusa SNC Repo

# This is the location of the Medusa repository
MEDUSA_PATH=/home/ms6401/projects/Medusa

# These are the locations of your desired directories
GEOIMAGE_HOME=/home/ms6401/projects/Code_Sandbox/geographic/geo_image
GEOIMAGE_DEST=${MEDUSA_PATH}/GeoImage

ORTHO_HOME=/home/ms6401/projects/Code_Sandbox/geographic/ortho_project
ORTHO_DEST=${MEDUSA_PATH}/OrthoProject



# Make sure source directories exists
if [ ! -d "${GEOIMAGE_HOME}" ]; then
    echo "${GEOIMAGE_HOME} does not exist, please fix error"
    exit
fi    
if [ ! -d "${ORTHO_HOME}" ]; then
    echo "${ORTHO_HOME} does not exist, please fix error"
    exit
fi

# Make sure destination directories exists
if [ ! -d "${GEOIMAGE_DEST}" ]; then 
    mkdir ${GEOIMAGE_DEST}
fi
if [ ! -d "${ORTHO_DEST}" ]; then 
    mkdir ${ORTHO_DEST}
fi

####################################################
#                 Geo Image  
####################################################
# MAKEFILE
cp ${GEOIMAGE_HOME}/Makefile ${GEOIMAGE_DEST}/

# SOURCE DIRECTORY
cp -r ${GEOIMAGE_HOME}/src  ${GEOIMAGE_DEST}/

# DATA DIRECTORY
cp -r ${GEOIMAGE_HOME}/data ${GEOIMAGE_DEST}/




#####################################################
#                     ORTHO   
#####################################################
# SOURCE DIRECTORY
cp -r ${ORTHO_HOME}/src  ${ORTHO_DEST}/

# DATA DIRECTORY
cp -r ${GEOIMAGE_HOME}/data ${GEOIMAGE_DEST}/



