#! /usr/bin/env bash

BUILD_PATH=/opt/local

GDAL_REPOSITORY="https://svn.osgeo.org/gdal/trunk/gdal"

GDAL_CONFIGURE_FLAGS="--prefix=${BUILD_PATH}   --with-java=$JAVA_HOME \
                      --with-jasper=/opt/local --with-jpeg12"



echo running gdal script


usage(){

cat <<EOF

Usage:
    `basename $0` download
    `basename $0` configure
    `basename $0` build
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
    
    build )
        echo building gdal
        make -j2
        ;;

    update )
        echo updating recent gdal
        ;;

    * )
        usage

esac






