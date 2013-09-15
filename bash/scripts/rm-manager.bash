#!/bin/bash -

. ~/.scripts/initializeANSI.sh

NAME_WIDTH=36
COLUMN_WIDTH=42

delete_item( ){

    realname="$(echo $1 | cut -c16- )"
            
    echo "Are you sure you with to delete: $realname"
    read action
    
    if [ "$action" == "y" -o "$action" == "Y" ]; then
        rm -rf ~/.deleted_files/$FILE2DEL
        echo "$realname permanently removed"
        else
            echo "Action cancelled"
    fi
    read
            
}

# Create a loop to monitor our progress
KEEP_RUNNING=1
while [ $KEEP_RUNNING -eq 1 ]; do
    
    deleted_items=`ls ~/.deleted_files`
    
    clear
    echo -e "${boldon} Removal Manager${boldoff}"
    echo ""
    echo "Available Items"
    
    counter=1
    for item in $deleted_items; do
        
        # pull the real filename and make sure it fits in the columns
        realname="$(echo $item | cut -c16- )"
        if [ ${#realname} -gt $(($NAME_WIDTH - 2 )) ]; then
            realname=${realname:0:$(($NAME_WIDTH - 2))}
        fi
        
        # Convert the name to a date timestamp
        datetime="$(echo $item | cut -c1-14 | awk -F. '{ print $5"/"$4" at "$3":"$2":"$1 }')"
        
        # Format the output for the first chunk
        output="$counter) ${greenf}${realname}${reset}"
        echo -e -n "$output"
        
        # Create the spaces
        space_dist=$(($COLUMN_WIDTH-${#output}))
        for (( i=0; i<$space_dist; i++ )); do
            echo -n " "
        done

        # Print the timestamp
        echo "$datetime"

        # Increment Counter
        counter=$(($counter+1))

    done
    if [ "$deleted_items" == "" ]; then
        echo -e "${greenf}    no items in archive ${reset}"
    fi

    echo ""
    echo -e "Available Commands:  ${bluef}1-$(($counter - 1))${reset}: Delete the selected item, ${bluef}q/Q${reset}: Quit Program"
    echo -e "                     ${bluef}c${reset}: clear archive"
    echo -n "Select the desired option: "
    
    # Read the desired action
    read desired 
    
    case "$desired" in 

        "q"|"Q") 
            echo ""
            KEEP_RUNNING=0
            ;;
        [0-9]|[0-9][0-9]|[0-9][0-9][0-9])
            
            # Find the respective item
            if [ $desired -gt $(( $counter - 1)) ]; then 
                echo "ERROR:  Cannot chose an item greater than $(($counter-1))"
                read
            fi
            
            FILE2DEL=$(ls ~/.deleted_files | sed -n "${desired}p" )
            delete_item $FILE2DEL
            
            ;;
        "c"|"C")
            
            #
            echo "Are you sure you wish to clear the archive? This cannot be undone. (y/n - n default): "
            read action
            if [ "$action" == "y" ]; then
                rm -rf ~/.deleted_files/*
                echo "Action Completed"
            else
                echo "Action Cancelled"
            fi
            read
            ;;
        *)
            ;;

    esac
done

