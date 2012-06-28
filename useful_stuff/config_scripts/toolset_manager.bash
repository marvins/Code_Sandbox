#! /usr/bin/env bash

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

echo "Use [ ] keys or select letters to navigate"
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

            *)
                echo ${response}
                exit 1
                ;;

        esac
    done
    return 

}


main
