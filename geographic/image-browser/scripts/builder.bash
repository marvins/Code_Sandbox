#!/bin/bash

QMAKE_EXE=qmake-qt4

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

######################################
#           Make Project             #
######################################
make_project(){

    #  Running qmake
    echo 'Running qmake'
    $QMAKE_EXE

    #  Running make
    echo 'Running make'
    make
    if [ ! "$?" == "0" ]; then
        echo "Error with last step, halting"
        ERROR_STATE=1
        exit 1
    fi
    
    #  Copying Data
    echo "Copying support files"
    cp -r src/html release/
    cp scripts/ImageBrowser.bash release/ImageBrowser
    cp -r src/icons release/


}

######################################
#           Clean Projet             #
######################################
clean_project(){

    # Cleaning project
    echo "running make clean"
    make clean

}

#------------------------------------#
#-          Main Function           -#
#------------------------------------#

# Required flags to check
HELP_FLAG_SET=0
MAKE_FLAG_SET=0
CLEAN_FLAG_SET=0


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


