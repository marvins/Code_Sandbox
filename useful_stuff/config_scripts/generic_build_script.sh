#! /usr/bin/env bash

echo running gdal script


usage(){

cat <<EOF

Usage:

    `basename $0` install
    `basename $0` update
    `basename $0` build
EOF
}


case "$1" in

    install )
        echo installing recent gdal
        ;;

    update )
        echo updating recent gdal
        ;;

    * )
        usage

esac






