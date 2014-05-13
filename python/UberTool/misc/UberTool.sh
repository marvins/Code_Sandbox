#!/bin/bash 
#

#  Get the path to where we are at now
pushd `dirname $0`/../share/UberTool > /dev/null
SCRIPTPATH=`pwd`

#  Fire off the ubertool
$SCRIPTPATH/UberTool.py

#  Return
popd > /dev/null

