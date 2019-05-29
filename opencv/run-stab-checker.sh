#!/bin/sh

#INPUT_VIDEO='data/IR_20190404175702_0.mpeg2'
INPUT_VIDEO='/media/irad/ms6401/stab_testing/stab_homography_ws2000.avi'

./debug/bin/opencv-video-stab-checker -c data/stab-checker.cfg -v ${INPUT_VIDEO}

