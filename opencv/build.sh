#!/bin/bash

OUTPUT_VIDEO=output.mp4
IMAGE_LIST=image-list.txt

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

./release/bin/video-writer -v $OUTPUT_VIDEO -i $IMAGE_LIST --overlay-path -ox 3080 -oy 3920 -s 0.5

