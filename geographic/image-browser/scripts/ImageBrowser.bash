#!/bin/bash


#  Find out where the executable is
BASE_PATH=`dirname $0`
#LIB_PATH=`pwd`/`dirname $BASE_PATH`/Libs

export LD_LIBRARY_PATH=$LIB_PATH:$LD_LIBRARY_PATH
export DYLD_LIBRARY_PATH=$LIB_PATH:$DYLD_LIBRARY_PATH

cd `dirname $0`
pwd
./`basename $0`.bin $@

