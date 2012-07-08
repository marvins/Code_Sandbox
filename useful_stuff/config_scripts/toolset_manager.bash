#! /usr/bin/env bash

# good command 
#cat toolset_manager.cfg | grep OPENCV_BUILD_LOCATION | sed -e 's/OPENCV_BUILD_LOCATION=//g'
#------------------------------------------------------------------------------
#----    GDAL Management Screen
#------------------------------------------------------------------------------
gdal_manager()
{

position=0
while :
    do  
        clear
        
        echo GDAL Management Menu

        echo Press q to exit

        read -n1 response

        case "${response}" in
            
            q|Q)
                echo
                return
                ;;

            *)
                echo ${response}
                exit 1
                ;;

        esac
    done
    return 

}

#------------------------------------------------------------------------------
#----    OpenCV Management Screen
#------------------------------------------------------------------------------
opencv_manager()
{

position=0
while :
    do  
        clear
        
        echo OpenCV Management Menu

        echo Press q to exit

        read -n1 response

        case "${response}" in
            
            q|Q)
                echo
                return
                ;;

            *)
                echo ${response}
                exit 1
                ;;

        esac
    done
    return 

}

#-------------------------------------------------------------------------------
#-- Procedure to display main menu
#-------------------------------------------------------------------------------

display_main_menu ()
{
    if [ -z "$1" ]
    then 
        echo "-Parameter 1 does not exist"
        exit
    else
        pos=$1
    fi

echo GeoImage Build Management Tool
echo
echo Operations:
echo Operation   Operation Description     Current Selection
echo ---------   ---------------------     -----------------
if [ $pos -eq 0 ]; then 
    echo "O           Manage OpenCV Install     <---"
else                    
    echo "O           Manage OpenCV Install"
fi
if [ $pos -eq 1 ]; then 
    echo "G           Manage GDAL Install       <---"
else
    echo "G           Manage GDAL Install"
fi
if [ $pos -eq 2 ]; then
    echo "Q           Quit                      <---"
else    
    echo "Q           Quit"
fi

echo "Use <[ ]> keys for <up down> or select letters to navigate"
echo "Use <x> for selection"
echo
}


arrowdown='\[B]'

#--------------------------------------------------------------------------------
#----  Main Driver
#--------------------------------------------------------------------------------
main(){

position=0
while :
    do  
        clear
        display_main_menu $position
        echo -n "Enter operation to perform: "
        read -n1 response

        case "${response}" in
            
            o|O)
                echo
                ;;
            g|G)
                echo
                ;;
            q|Q)
                echo
                exit 1
                ;;

            ']')
                position=$[ position + 1 ]
                if [ "$position" -gt "2" ]; then
                    position=0
                fi
                ;;
            '[')
                position=$[ position - 1 ]
                if [ "$position" -lt "0" ]; then
                    position=2
                fi
                ;;
            x|X)
                if [ "$position" -eq "0" ]; then
                    opencv_manager
                elif [ "$position" -eq "1" ]; then
                    gdal_manager
                else
                    exit 1
                fi
                ;;

            *)
                echo ${response}
                exit 1
                ;;

        esac
    done
    return 

}



# Parsing agent
if [ -z "$1" ]
    main

