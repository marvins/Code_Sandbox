#!/bin/bash

OUTPUT_VIDEO=output.mp4
IMAGE_LIST=image-list.txt

mkdir -p release

pushd release

cmake ..

make -j4

popd

./release/bin/video-writer $OUTPUT_VIDEO $IMAGE_LIST

