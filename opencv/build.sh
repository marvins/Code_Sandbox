#!/bin/bash

OUTPUT_VIDEO=output.mp4
IMAGE_LIST=image-list.txt

#OX=1080
#OY=920

#  16K x 16K
OX="`expr 8192 - 1920`"
OY="`expr 8192 - 1080`"


#  Create directory
mkdir -p release


#  Enter Directory
pushd release


#  Run CMake
cmake ..
if [ ! "$?" = '0' ]; then
    echo 'Error with CMake.'
    exit 1
fi


make -j4
if [ ! "$?" = '0' ]; then
    echo 'Error with Make.'
    exit 1
fi

popd


CMD="./release/bin/video-writer -v $OUTPUT_VIDEO -i $IMAGE_LIST --overlay-path -ox $OX -oy $OY -s 1"
echo $CMD
$CMD

