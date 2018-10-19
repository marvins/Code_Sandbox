#!/bin/bash

OUTPUT_VIDEO=output.mp4
IMAGE_LIST=image-list.txt

#OX=1080
#OY=920

#  16K x 16K
OX=8000
#"`expr 1024`"
OY=9000
#"`expr  2500`"

SCALE=1

FPS=4

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


CMD="./release/bin/video-writer -v $OUTPUT_VIDEO -i $IMAGE_LIST --overlay-path -ox $OX -oy $OY -s $SCALE -fps $FPS -gui"
echo $CMD
$CMD

