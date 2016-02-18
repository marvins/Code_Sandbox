#!/bin/bash

QMAKE_EXE='qmake-qt4'

#################################
#             Usage             #
#################################
usage(){

    echo ""
    echo "usage: $0 [options]" 
    echo ""
    echo "    options:"
    echo ""
    echo "      - help : Print usage"
    echo ""
    echo "      - make :  Build software"
    echo ""
    echo "      - clean : Clean up everything"
    exit 1
}

################################################
#          Load the Build Environment          #
################################################
load_environment(){

    echo 'Loading Environment Variables'
}

######################################
#           Make Project             #
######################################
make_project(){

    #  Clean Data
    if [ "$OS" == "Darwin" ]; then
        if [ -d "release/ImageBrowser.app" ]; then
            echo 'cleaning application'
            rm -rf release/ImageBrowser.app
        fi
    fi

    #  Running qmake
    echo 'Running qmake'
    qmake

    #  Running make
    echo 'Running make'
    make
    if [ ! "$?" == "0" ]; then
        echo "Error with last step, halting"
        ERROR_STATE=1
        exit 1
    fi
    
    #  Copying Data
    if [ "$OS" == "Linux" ]; then
        echo "Copying support files"
        cp -r src/html release/
        cp scripts/ImageBrowser.bash release/ImageBrowser
        cp -r src/icons release/
    elif [ "$OS" == "Darwin" ]; then
        echo 'Copying Mac Support Files'
        cp -r src/html  release/ImageBrowser.app/Contents/MacOS/
        cp -r src/icons release/ImageBrowser.app/Contents/MacOS/
        cp release/ImageBrowser.app/Contents/MacOS/ImageBrowser release/ImageBrowser.app/Contents/MacOS/ImageBrowser.bin
        cp scripts/ImageBrowser.bash release/ImageBrowser.app/Contents/MacOS/ImageBrowser
    fi


}

######################################
#           Clean Projet             #
######################################
clean_project(){

    # Cleaning project
    echo "running make clean"
    make clean

    # Removing release
    rm -rf release debug

}


#------------------------------------#
#-          Main Function           -#
#------------------------------------#

# Required flags to check
HELP_FLAG_SET=0
MAKE_FLAG_SET=0
CLEAN_FLAG_SET=0

#  Load default Environment
. scripts/systems/default.bash

#  Determine our operating system
OS='Linux'
if [ "`uname`" == "Darwin" ]; then
    OS='Darwin'
    . scripts/systems/darwin.bash 
fi

if [ "$OS" == "Linux" ]; then
    load_environment
fi


#-----------------------------------------#
#     Process command-line arguments      #
for ARG in $@; do

    case $ARG in 
        
        #------------------#
        #-    Help Flag   -#
        "-h" | "-help" | "help" )
            HELP_FLAG_SET=1
            ;;

        #------------------#
        #-    Make Flag   -#
        "-make" | "make" )
            MAKE_FLAG_SET=1
            ;;
        
        #-----------------------#
        #-    Clean Flag Set   -#
        "-clean" | "clean")
            CLEAN_FLAG_SET=1
            ;;

        #----------------------#
        #-   Unknown Option   -#
        *)
            echo "error: unknown option $ARG"
            usage
            ;;
    
    esac

done


#  Print Help
if [ "$HELP_FLAG_SET" == "1" ]; then
    usage
fi

#  Run Make
if [ "$MAKE_FLAG_SET" == "1" ]; then
    make_project
fi

#  Run Clean
if [ "$CLEAN_FLAG_SET" == "1" ]; then
    clean_project
fi


