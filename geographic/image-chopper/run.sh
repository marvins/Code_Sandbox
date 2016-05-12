#!/usr/bin/env bash

#./find-bounds.py -d /data/9Z2010IR2_gs2ir_20160303_181334 -o /data/9Z2010IR2_gs2ir_20160303_181334_mosaic/tiles

#  Define Paths
BASE_PATH=/data/9Z2010IR2_gs2ir_20160303_181334
BASE_PATH_FLAG=0

#  Skip Finding Bounds if You want to ease suffering
SKIP_FIND_BOUNDS='0'

#  Set to 1 if you want to wipe out all existing data
CLEAN_DATA='0'

#  Number of Threads
NUM_THREADS=1
NUM_THREADS_FLAG=0


#  Tile Size In Meters
TSM='256'
TSM_FLAG=0

#  Ground Sampling Distance
GSD='0.25'

IMAGE_BOUNDS='-minx 512700 -miny 4308231 -maxx 528046.8 -maxy 4384000.7'

PASS_THROUGH_ARGS=''
PASS_THROUGH_FLAG=0
DRY_RUN=0

#----------------------------------------#
#-      Parse Command-Line Options      -#
#----------------------------------------#
for ARG in "$@"; do
    case $ARG in

        #  Clean Data
        '-c')
            CLEAN_DATA=1
            ;;

        #  SEt number of threads
        '-nt')
            NUM_THREADS_FLAG=1
            ;;

        #  Override the Base Path
        '-b')
            BASE_PATH_FLAG=1
            ;;

        #  Override the Tile-Size Flag
        '-tsm')
            TSM_FLAG=1
            ;;
        
        #  Add Remaining
        '--pass-through'|'-ps')
            PASS_THROUGH_FLAG=1
            ;;

        #  Request Dry Run
        '--dry-run')
            PASS_THROUGH_ARGS="$PASS_THROUGH_ARGS --dry-run"
            DRY_RUN=1
            ;;

        #  Unknown option
        *)
            #  Pass-through arguments
            if [ "$PASS_THROUGH_FLAG" = '1' ]; then
                PASS_THROUGH_ARGS="$PASS_THROUGH_ARGS $ARG"

            #  Num-Threads
            elif [ "$NUM_THREADS_FLAG" = '1' ]; then
                NUM_THREADS_FLAG=0
                NUM_THREADS=$ARG

            #  Base Path Flag
            elif [ "$BASE_PATH_FLAG" = '1' ]; then
                BASE_PATH_FLAG=0
                BASE_PATH=$ARG

            #  Tile-Size Flag
            elif [ "$TSM_FLAG" = '1' ]; then
                TSM_FLAG=0
                TSM=$ARG
            #  Error
            else
                echo "error: Unknown argument ($ARG)"
                exit 1
            fi
            ;;
    esac
done


#   Create Subdirectories
INPATH=${BASE_PATH}/transform-utility
TEMP_PATH=${BASE_PATH}/temp
TILE_PATH=${BASE_PATH}/tiles
VRT_PATH=${BASE_PATH}/gs2ir_20160303_181334

#  Print Status
echo "Base Path : $BASE_PATH"
echo "Img Path  : $INPATH"
echo "Temp Path : $TEMP_PATH"
echo "Tile Path : $TILE_PATH"
echo "Output Img: $VRT_PATH"

#  Create Working DIrectories
mkdir -p ${TEMP_PATH}
mkdir -p ${TILE_PATH}

#  Check if we should clean
if [ "$CLEAN_DATA" = '1' ]; then

    if [ -d "${TEMP_PATH}" ]; then
        rm -r ${TEMP_PATH}/*
    fi

    if [ -d "${TILE_PATH}" ]; then
        rm -r ${TILE_PATH}/*
    fi
fi

#  Start Constructing Tiles
if [ "$SKIP_FIND_BOUNDS" = '0' ]; then
    ./find-bounds.py -d ${INPATH} -o ${TILE_PATH} -t ${TEMP_PATH}  -s -tsm ${TSM} -gsd ${GSD} ${IMAGE_BOUNDS} -nt ${NUM_THREADS} ${PASS_THROUGH_ARGS}
fi

#  Enter Base Directory
pushd ${BASE_PATH}

#  Get list of Frame 0 Files
IMG_TILES=`ls tiles/*.tif`

#  Build Overviews
echo 'Building Overviews'
if [ "$DRY_RUN" = '1' ]; then
    for X in `ls ${TILE_PATH/*.tif`; do
        CMD=gdaladdo -r cubic $X 2 4 8 16 32
        echo $CMD
    done
else
    ls ${TILE_PATH}/*.tif | parallel gdaladdo -r cubic {} 2 4 8 16
fi


if [ "$DRY_RUN" = '1 ']; then
    echo "Running Dry Run, Skipping Virtual Dataset."
    exit 0
fi

#  Build the Virtual Dataset
if [ -f "$VRT_PATH.vrt" ]; then
    rm $VRT_PATH
fi

#  Create the Virtual Dataset
CMD="gdalbuildvrt ${VRT_PATH}.vrt $IMG_TILES"
echo $CMD
$CMD

#  Create the Tile Index
CMD="gdaltindex   ${VRT_PATH}.shp $IMG_TILES"
echo $CMD
$CMD

popd
