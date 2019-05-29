#!/bin/sh

#ARGLIST='--ws=yes --ws-local-outlier-rejection=yes'
#ARGLIST='--mosaic=yes --ws=yes --ws-local-outlier-rejection=yes'
#ARGLIST='--mosaic=yes --ws=yes --ws-local-outlier-rejection=yes -m=homography'
#ARGLIST='--mosaic=yes --ws=yes --ws-local-outlier-rejection=yes --ws-nkps=2000 -m=homography'
#ARGLIST='--mosaic=yes --ws=yes --ws-local-outlier-rejection=yes --ws-nkps=2000 -m=homography -r=30 -nkps=2000'
#ARGLIST='--mosaic=yes --ws=yes --ws-local-outlier-rejection=yes --ws-nkps=2000 -m=homography -r=10 -nkps=2000'
ARGLIST='--mosaic=yes --ws=yes --ws-local-outlier-rejection=yes --ws-nkps=2000 -m=homography -r=100 -nkps=2000'

OUT_PATH=stab_homography_ws2000_r100_nkps2000.avi

./debug/bin/opencv_video_stab_temp ${ARGLIST} data/IR_20190404175702_0.mpeg2 -o=${OUT_PATH}

