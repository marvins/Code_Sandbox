#!/bin/sh
#
#    File:    deploy.sh
#    Author:  Marvin Smith
#    Date:    5/12/2014
#
#    Purpose: Deploy the UberTool to a specific location
#


PREFIX="$HOME/local"


PREFIX_CLEAN=`echo $PREFIX | sed 's/\//\\\\\//g'`


#   Test if the bin directory exists
if [ ! -d "$PREFIX/bin" ]; then
    mkdir -p "$PREFIX/bin"
fi

#   Test if the share directory exists
if [ ! -d "$PREFIX/share" ]; then
    mkdir -p "$PREFIX/share"
fi

#  Copy the UberTool materials to the share path
if [ ! -d "$PREFIX/share/UberTool" ]; then
    mkdir -p "$PREFIX/share/UberTool"
else
    rm -rf "$PREFIX/share/UberTool"
    mkdir -p "$PREFIX/share/UberTool"
fi

# copy the main script
cp src/UberTool.py $PREFIX/share/UberTool/UberTool.py
cp misc/UberTool.sh $PREFIX/share/UberTool/UberTool.sh

# Copy other utilities
cp -r src/plugins $PREFIX/share/UberTool/
cp -r src/core    $PREFIX/share/UberTool/
cp -r src/gui     $PREFIX/share/UberTool/
cp -r src/icons   $PREFIX/share/UberTool/

#  Create the symbolic link
if [ -f "$PREFIX/bin/UberTool" ]; then
    rm $PREFIX/bin/UberTool
fi
ln -s $PREFIX/share/UberTool/UberTool.sh $PREFIX/bin/UberTool

#  Copy the desktop link
sed "s/__PREFIX__/$PREFIX_CLEAN/g" misc/UberTool.desktop.template > misc/UberTool.desktop
chmod +x misc/UberTool.desktop

