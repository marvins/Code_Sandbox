#! /usr/bin/env bash

GDAL_REPOSITORY="https://svn.osgeo.org/gdal/trunk/gdal"

GDAL_CONFIGURE_FLAGS="--with-java=yes"



echo running gdal script


usage(){

cat <<EOF

Usage:
    `basename $0` download
    `basename $0` configure
    `basename $0` install
    `basename $0` update
EOF
}


case "$1" in

    download )
        svn checkout ${GDAL_REPOSITORY}
        ;;

    configure )
        ./configure  ${GDAL_CONFIGURE_FLAGS}
        
        ;;

    update )
        echo updating recent gdal
        ;;

    * )
        usage

esac






