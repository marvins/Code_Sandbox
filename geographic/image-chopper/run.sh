#!/usr/bin/env bash

#./find-bounds.py -d /data/9Z2010IR2_gs2ir_20160303_181334 -o /data/9Z2010IR2_gs2ir_20160303_181334_mosaic/tiles

#  Define Paths
INPATH=/data/9Z2010IR2_gs2ir_20160303_181334
OUTPATH=/data/9Z2010IR2_gs2ir_20160303_181334_mosaic
TILE_PATH=/data/9Z2010IR2_gs2ir_20160303_181334_tiles
VRT_PATH=/data/gs2ir_20160303_181334.vrt

#  Skip Finding Bounds if You want to ease suffering
SKIP_FIND_BOUNDS='0'


#  Tile Size In Meters
TSM='256'

#  Ground Sampling Distance
GSD='0.25'

#  Start Constructing Tiles
if [ "$SKIP_FIND_BOUNDS" = '0' ]; then
    ./find-bounds.py -d ${INPATH} -o ${TILE_PATH} -s -tsm ${TSM} -gsd ${GSD}
fi

#  Get list of Frame 0 Files
IMG_TILES=`ls ${TILE_PATH}/frame_00000_imager_*.tif`

#  Build Overviews
for IMG_TILE in $IMG_TILES; do
    gdaladdo -r cubic $IMG_TILE 2 4 8 16
done

#  Build the Virtual Dataset
if [ -f "$VRT_PATH" ]; then
    rm $VRT_PATH
fi

gdalbuildvrt $VRT_PATH $IMG_TILES
