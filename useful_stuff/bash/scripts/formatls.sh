#!/bin/sh


function gmk()
{
    # Given input in Kb, output in Kb, Mb, or Gb for best output format
    if [ $1 -ge 1000000 ] ; then
        echo "$(scriptbc -p 2 $1 / 1000000)Gb"
    elif [ $1 -ge 1000 ] ; then
        echo "$(scriptbc -p 2 $1 / 1000)Mb"
    else
        echo "${1}Kb"
    fi
}


if [ $# -gt 1 ] ; then
    echo "Usage: $0 [dirname]" >&2; 
    exit 1
elif [ $# -eq 1 ]; then
    cd "$@"
fi

echo 0 > MAXLEN

# Get the longest length
for file in *
do
    LEN=$(cat MAXLEN)
    if [ "$(echo -n $file | wc -m)"  -gt "$LEN" ]; then
        echo ${#file} > MAXLEN
    fi
done

# Set the max length
MAXLEN=$(cat MAXLEN)

# Iterate through files
for file in *
do 
    output1=""
    output2=""
    # Take separate action depending on file or directory
    if [ -d $file ]; then

        size="$(ls "$file" | wc -l | sed 's/[^[:digit:]]//g')"
        
        if [ $size -eq 1 ]; then
            output1="$file"
            output2="($size entry)"
        else
            output1="$file"
            output2="($size entries)"
        fi
    else
        size="$(ls -sk "$file" | awk '{print $1}')"
        output1="$file"
        output2="($(gmk $size))" 
    fi
    
    # replace the carrots with spaces
    STRLEN=$(( $MAXLEN - ${#output1} + 4 ))
    
    echo -n $output1
    for i in `seq 1 $STRLEN`;
    do
        echo -n ' '
    done
    echo $output2

done 

if [ -f MAXLEN ]; then
    rm MAXLEN
fi

exit 0


