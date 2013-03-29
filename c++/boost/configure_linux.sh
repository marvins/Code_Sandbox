#!/bin/bash

PREFIX=/opt/local

read -p "Do you wish to run the bootstrap script? (y/n - default): "  ANS
if [ $ANS = 'y' ]  || [ $ANS = 'Y' ]; then 
    ./bootstrap.sh --prefix=$PREFIX
fi

read -p 'Do you wish to build boost? (y/n - default): ' ANS
if [ $ANS = 'y' ]  || [ $ANS = 'Y' ]; then 
    echo "WORLD" #sudo ./b2 --install
fi

