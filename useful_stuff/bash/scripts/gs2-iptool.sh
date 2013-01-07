#! /bin/bash -


# Global Variables
ERROR_ON=1
ERROR_OFF=0


#################################
#        Help Function          #
#################################
help( ){

cat <<EOF
usage: gs2-assets [args]
    
    args: 
        NONE: system will search for all system components plus local hostfile
        GSMC/gsmc      : system will search for all entries in GSMC host file
        GSAM/gsam      : system will search for all entries in GSAM host file
        APN/apn/POD/pod: system will search for all entries in the airborne network
        LOCAL/local    : system will search for all entries in the local machine's host file
        FULL/full      : system will search for all system components plus local hostfile

EOF


}

###################################
#        Error Function           #
###################################
error( ){

    echo $1
    help

    return $ERROR_ON
}


##############################################
#        Build List of Host Entries          #
##############################################
build_hostlist( ){
    
    # GSMC Systems
    if [ "$1" == "GSMC" ]; then
        echo "172.16.0.11  gsmc-server1" >> tempfile
        echo "172.16.2.11  gsmc-server1-twister" >> tempfile
        echo "172.16.0.12  gsmc-server2" >> tempfile
        echo "172.16.2.12  gsmc-server2-twister" >> tempfile
        echo "172.16.0.21  gsmc-ws1" >> tempfile
        echo "172.16.2.21  gsmc-ws1-twister" >> tempfile
        echo "172.16.0.22  gsmc-ws2" >> tempfile
        echo "172.16.2.22  gsmc-ws2-twister" >> tempfile
        echo "172.16.0.23  gsmc-ws3" >> tempfile
        echo "172.16.2.23  gsmc-ws3-twister" >> tempfile

    # GSAM Systems
    elif [ "$1" == "GSAM" ]; then
        echo "172.16.16.11 gsam-server" >> tempfile
        echo "172.16.16.21 gsam-ws" >> tempfile
   
    # GSN Systems
    elif [ "$1" == "GSN" ]; then
        echo "172.16.2.21  hsl1" >> tempfile
        echo "172.16.2.22  hsl2" >> tempfile
        echo "172.16.2.23  hsl3" >> tempfile

    # Local Systems
    elif [ "$1" == "LOCAL" ]; then
        
        # 1. cat the hostfile
        # 2. Remove empty lines
        # 3. Remove multiple spaces
        # 4. Keep only the first two entries
        cat "/etc/hosts" | sed '/^$/d' | sed 's/^ *//;s/ *$//;s/ \{1,\}/ /g' | cut -d ' ' -f1,2 >> tempfile 

    # Full Systems
    elif [ "$1" == "FULL" ]; then
        build_hostlist "GSMC"
        build_hostlist "GSAM"
        build_hostlist "LOCAL"
    
    # Error condition
    else 
        error "Unknown condition: $1"
    fi

}


######################################
#      Clean out the temp file       #
######################################
clean_tempfile( ){
    
    # make sure the file exists
    if [ ! -f tempfile ]; then
        return
    fi
    NUMS=""

    # Find all localhost entries
    NUMS="$NUMS `grep -n "localhost" tempfile | cut -d ':' -f 1`"
    
    # 
    NUMS="$NUMS 1"

    # SORT NUMS
    NUMS=`echo $NUMS | fmt -1 | sort`

    # Delete all suspect lines
    COUNT=0
    for NUM in $NUMS; do

        CURRENT_VAL=$(($NUM - $COUNT))
        sed "${CURRENT_VAL}d" tempfile >> tmpfile;  mv tmpfile tempfile

        # Increment Count
        COUNT=$(($COUNT+1))

    done

}


#############################
#       Print Center        #
#############################
print_entry( ){

    # Data Variables
    LINE=$1
    SIZE=$((${#1}))
    RSIZE=$2
    
    # Center Alignment
    if [ "$3" == "CENTER" ]; then
        STARTW=$((($RSIZE - $SIZE)/2))
        ENDW=$(($RSIZE-$STARTW))
    
    elif [ "$3" == "LEFT" ]; then
        STARTW=0
        ENDW=$(($STARTW + $SIZE))
    
    else
        error "UNKNOWN OPTION: $3"
    fi
    
    # Set the start and end
    if [ $(($ENDW-$STARTW)) -gt $SIZE ]; then
        ENDW=$(($ENDW-1))
    fi
    
    # Iterate through the output
    for ((j=0; j<$RSIZE; j++ )); do
        if [ $j -eq $STARTW ]; then
            IDX=$(($j-$STARTW))
            echo -n "${LINE[$IDX]}"
        
        elif [ $j -gt $STARTW ] && [ $j -lt $ENDW ]; then
            echo -n ""
        
        else
            echo -n " "
        
        fi

    done
    

}

##############################
#     Print table header     #
##############################
print_table_header( ){

    header_size=$((4 + $4 + 7 + $5 + 7 + $6 + 2 ))
    p1=$(($4+6))
    p2=$(($5+7+$p1))

    # Top Row of Header
    echo -n '+'
    for ((i=0; i<$header_size; i++ )); do

        if [ $i -eq $p1 -o $i -eq $p2 ]; then
            echo -n '+'
        else
            echo -n '-'
        fi

    done
    echo -n '+'
    echo ''

    echo -n '|   '

    print_entry $1 $4 "CENTER"

    echo -n '   |   '
    
    print_entry $2 $5 "CENTER"

    echo -n '   |   '
    
    print_entry $3 $6 "CENTER"

    echo '   |'

    # Bottom Row of Header
    echo -n '+'
    for ((i=0; i<$header_size; i++ )); do
        if [ $i -eq $p1 -o $i -eq $p2 ]; then
            echo -n '+'
        else
            echo -n '-'
        fi
    done
    
    echo -n '+'
    echo ''

}

##############################
#     Print table footer     #
##############################
print_table_footer( ){

    header_size=$((2 + $4 + 7 + $5 + 7 + $6 + 4 ))
    p1=$(($4+6))
    p2=$(($5+7+$p1))

    # Top Row of Header
    echo -n '+'
    for ((i=0; i<$header_size; i++ )); do
       
        if [ $i -eq $p1 -o $i -eq $p2 ]; then
            echo -n '+'
        else 
            echo -n '-'
        fi
    done
    echo -n '+'
    echo ''
}

#############################
#     Print a table row     #
#############################
print_table_row( ){

    echo -n "|   "
    print_entry $1 $4 "LEFT"
    echo -n "   |   "
    print_entry $2 $5 "LEFT"
    echo -n "   |   "
    print_entry $3 $6 "CENTER"
    echo -n "   |"
    
    
    echo ""
}


#########################################
#      Print the IP Table Results       #
#########################################
print_table( ){

    # pull the input args
    output=$1
    COUNT=$2

    # Find the max length of each column
    MAX_0=0
    MAX_1=0
    MAX_2=0
    NAME_0=$4
    NAME_1=$5
    NAME_2=$6

    for (( i=0; i<$COUNT; i++ )); do
        A0=${#output[$(($i * 3 + 0))]}
        if [ $A0 -gt $MAX_0 ]; then
            MAX_0=$A0
        fi

        A1=${#output[$(($i * 3 + 1))]}
        if [ $A1 -gt $MAX_1 ]; then
            MAX_1=$A1
        fi

        A2=${#output[$(($i * 3 + 2))]}
        if [ $A2 -gt $MAX_2 ]; then
            MAX_2=$A2
        fi


    done
    
    # Increase column width if name is longer
    if [ $MAX_0 -lt ${#NAME_0} ]; then
        MAX_0=${#NAME_0}
    fi
    if [ $MAX_1 -lt ${#NAME_1} ]; then
        MAX_1=${#NAME_1}
    fi
    if [ $MAX_2 -lt ${#NAME_2} ]; then
        MAX_2=${#NAME_2}
    fi

    # Print the table rows
    print_table_header $4 $5 $6 $MAX_0 $MAX_1 $MAX_2
    for ((i=0; i<$COUNT; i++ )); do
        print_table_row ${output[$(($i*3+0))]} ${output[$(($i*3+1))]} ${output[$(($i*3+2))]} $MAX_0 $MAX_1 $MAX_2
    done
    print_table_footer $4 $5 $6 $MAX_0 $MAX_1 $MAX_2

}


##############################################
#      Search the GS2 System for assets      #
##############################################
find_gs2_assets( ){

    # Check if the user gave us a hostfile preference
    GS2_SYSTEM=""
    
    # User wants to check the full system
    if [ $# -lt 1 ]; then
        GS2_SYSTEM="FULL"

    # User wants to check a specific item
    elif [ $# -ge 1 ]; then

        case "$1" in 
        
            "GSMC"|"gsmc")
                GS2_SYSTEM="GSMC"
                ;;

            "GSAM"|"gsam")
                GS2_SYSTEM="GSAM"
                ;;

            "GSN"|"gsn")
                GS2_SYSTEM="GSN"
                ;;

            "POD"|"pod"|"APN"|"apn")
                GS2_SYSTEM="APN"
                ;;

             "FULL"|"full")
                GS2_SYSTEM="FULL"
                ;;

             "LOCAL"|"local")
                GS2_SYSTEM="LOCAL"
                ;;

             *)
                error "ERROR: $1 is not a valid input"
                return $ERROR_ON
                ;;
        esac
    
    fi

    
    # If temporary file exists, then remove it
    if [ -f "tempfile" ]; then
        rm tempfile
    fi
    
    # Select the list of items to compare
    case $GS2_SYSTEM in 
    
        "GSMC")
            build_hostlist ${GS2_SYSTEM}
        ;;
        
        "LOCAL")
            build_hostlist ${GS2_SYSTEM}
        ;;
        
        "FULL")
            build_hostlist ${GS2_SYSTEM}
        ;;

    esac
    
    # Clean out bad entries
    clean_tempfile

    # Convert to an array
    if [ -f "tempfile" ]; then
        DATA=`cat tempfile`
        rm tempfile
    fi
    
    # Iterate through the list, testing each IP Address
    OUTPUT=""
    COUNT=0
    for item in $DATA; do COUNT=$(($COUNT+1)); done
    COUNT=$(($COUNT/2))
    data=($DATA)
    for ((i=0; i<$COUNT; i++ )); do
        
        # Useful Variables
        IPADDR="${data[$(($i * 2 + 0))]}"
        IPNAME="${data[$(($i * 2 + 1))]}"
        RES="Non-Responsive"

        # Test the ip address by using ping
        ping -W 1 -c 1 $IPADDR > /dev/null
        
        if [ "$?" == "0" ]; then
            RES="Responsive"
        fi

        # Add results to output
        OUTPUT="$OUTPUT $IPNAME $IPADDR $RES"

    done
    
    # Create an iterable array
    output=($OUTPUT)
    
    # Print the table
    print_table $output $COUNT 3 "hostname" "ip-address" "result"

}

############################
#  Main Function   
############################
find_gs2_assets $@

