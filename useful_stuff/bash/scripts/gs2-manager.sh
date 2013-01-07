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
    
    # Give a set of options if in GUI mode
    OPTION=0
    while [ $GUI -eq 1 ]; do

        # Print the header


        # Give the user input

    done
    if [ ! $GUI -eq 1 ]; then

    fi


}


# This is to allow the program to be run as a gui
gs2_manager "GUI" $@

