#!/bin/bash

# Parse all command-line options
flags=""
while getopts "f" opt
    do
        case $opt in
        f)
            rm -rf ~/.deleted_files/*
            cat "deleted on $(date)" >> /home/ms6401/Desktop/archived_cleared.txt
            exit 0
            ;;
        *)
        ;;
       esac
    done


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


