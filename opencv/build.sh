#!/bin/bash

OUTPUT_VIDEO=output.mp4
IMAGE_LIST=image-list.txt

OX=6272
OY=7112

mkdir -p release

pushd release

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


./release/bin/video-writer -v $OUTPUT_VIDEO -i $IMAGE_LIST --overlay-path -ox $OX -oy $OY -s 1

