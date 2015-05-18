#!/bin/sh

OUTPUT='output.png'
IMG_DIR=images
IMAGES=''

for IMG in `ls $IMG_DIR`; do
    IMAGES="$IMAGES -i $IMG_DIR/$IMG"
done

FEATURE_NAME='surf'

#  Build the Command
CMD="./stitcher $IMAGES $OUTPUT -f $FEATURE_NAME"
echo $CMD


#   Run the command
$CMD

