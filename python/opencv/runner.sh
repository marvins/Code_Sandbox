#!/bin/sh

#  Define our anchor image
ANCHOR_IMAGE='photos/p4.jpg'

#  Define our output image
OUTPUT_IMAGE='result.png'

#  Define the image list
IMAGES='photos/p1.jpg
        photos/p2.jpg
        photos/p3.jpg'
IMAGE_LIST=''

for IMAGE in $IMAGES; do
    IMAGE_LIST="$IMAGE_LIST --image $IMAGE"
done

#  Node Map
NODE_MAP='a -> b, b -> c, c -> d'

#  Run the mosaic builder
./MosaicBuilder.py --anchor-image=$ANCHOR_IMAGE \
                   --output-image=$OUTPUT_IMAGE \
                   $IMAGE_LIST \
                   --node-map "$NODE_MAP"
