#! /bin/sh

#  The main purpose of this script is to make it easier to package
#  releases for different operating systems.  Basically this script
#  copies libraries which are used and shoves them into a single 
#  directory.


#  Your choices are 
# 
#  UBUNTU
#  RHEL
DISTRO_TYPE="UBUNTU"


#  GDAL Dependencies
GDAL_LIBS="libgdal.so*"

OPENCV_LIBS=

for library in ${GDAL_LIBS}
    do 
        ls /usr/lib/${library}
    done




