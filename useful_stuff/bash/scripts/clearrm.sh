#!/bin/sh

echo -n "Are you sure you wish to clear the archive? (y/n) [n]: "
read desired

if [ ${desired:="n"} == "y" ]; then
    rm -rf ~/.deleted_files/*
    echo "Archive cleared"
    exit 0
else
    echo "Action canceled"
    exit 1
fi


