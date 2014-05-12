#!/bin/sh
#
#    File:    deploy.sh
#    Author:  Marvin Smith
#    Date:    5/12/2014
#
#    Purpose: Deploy the UberTool to a specific location
#


PREFIX=$HOME/local

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
fi

# copy the main script
cp UberTool.py $PREFIX/share/UberTool/UberTool.py

# Copy other utilities
cp -r add-ons $PREFIX/share/UberTool/
cp -r core    $PREFIX/share/UberTool/
cp -r gui     $PREFIX/share/UberTool/
cp -r icons   $PREFIX/share/UberTool/

#  Create the symbolic link
ln -s $HOME/local/share/UberTool/UberTool.py $HOME/local/bin/UberTool

