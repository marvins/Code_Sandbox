#!/bin/sh


###################################
#     GS2 Management Program      #
###################################
gs2_manager( ){

    # Set depending on GUI or not
    GUI=0
    if [ "$1" == "GUI" ]; then
        GUI=1
    fi



}


# This is to allow the program to be run as a gui
gs2_manager "GUI" $@

