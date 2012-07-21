#! /usr/bin/env bash

echo running opencv config

OPENCV_REPO_LOCATION="http://code.opencv.org/svn/opencv"

echo OpenCV Management Script


#############################################
#                   USAGE                   #
#############################################
usage(){

cat <<EOF

Usage:
    `basename $0` download
EOF
}


case "$1" in

    download )
        svn checkout ${OPENCV_REPO_LOCATION}
        ;;

    update )
        svn up
        ;;
    
    build )
        mkdir -p release
        cd release
        cmake .. 
        ;;
    * )
        usage

esac






