#!/bin/sh

# unrm - Searches the deleted file archive for the specified file or directory.
#    If there is more than one matching result, then it will show a list of results,
#    ordered by timestamp.  The user will then specify which file to restore.

# import the ANSI Color Codes
. ~/.scripts/initializeANSI.sh

# Set up some variables/alias'
mydir="$HOME/.deleted_files"
realrm="/bin/rm"
move="/bin/mv"

# Establish the destination
dest=$(pwd)

# Make sure the archive exists
if [ ! -d $mydir ]; then
    echo -e "${redf}$0: No archive exists at $mydir. Nothing to delete.${reset}"
fi

# Navigate the the archive
cd $mydir

# Check for arguments, if none, then show listing
if [ $# -eq 0 ]; then
    echo "Contents of your deleted files archive (sorted by date):"
    ls -FC | sed -e 's/\([[:digit:]][[:digit:]]\.\)\{5\}//g' -e 's/^/  /'
    exit 0
fi

# If there is an argument, then it is assumed to be the name of the pattern to match against.
matches="$(ls -d *"$1"* 2> /dev/null | wc -l)"

# Check for no matches
if [ $matches -eq 0 ]; then 
    echo "No match for \"$1\" in the deleted file archive"
    exit 1
fi

# Check for more than one match
if [ $matches -gt 1 ]; then
    echo "More than one file or directory match in the archive:"
    index=1
    for name in $(ls -td *"$1"*)
    do 
        # convert the filename into a timestamp string
        datetime="$(echo $name | cut -c1-14 | \
        awk -F. '{ print $5"/"$4" at "$3":"$2":"$1 }')"

        # convert the deleted name to the original name
        realname="$(echo $name | cut -c16- )"

        # Choose between file ane directory entries
        if [ -d $name ] ; then
            size="$(ls $name | wc -l | sed 's/[^[:digit:]]//g')"
            echo -e " $index) ${boldon}$realname${boldoff} (size = ${size}Kb, deleted = ${datetime})"
        fi
        if [ -f $name ]; then
            size="$(ls -sdk1 $name | awk '{print $1}')"
            echo -e " $index) $realname (size = ${size}Kb, deleted = ${datetime})"
        fi
        
        # increment the index
        index=$(($index + 1))

    done
    echo 
    echo -n "Which version of $1 do you wish to restore ('0' to quit)? [1]: "
    read desired

    # check if the number is too big
    if [ ${desired:=1} -ge $index ]; then
        echo "$0: Restore canceled by user: index value too big." >&2
        exit 1
    fi

    # Check if the number is less than 1
    if [ $desired -lt 1 ]; then
        echo "$0: Restore cancelled by user." >&2
        exit 1
    fi


    # Select the restore file
    restore="$(ls -td1 *"$1"* | sed -n "${desired}p")"
    oname="$dest/$(echo $restore | cut -c16- ).restored"
    if [ -e "$oname" ]; then
        echo "\"$1\" already exists in this directory. Cannot overwrite." >&2
        exit 1
    fi

    echo -n "Restoring file \"$oname\" ..."
    $move "$restore" "$oname"
    echo done

    echo -n "Delete additional copies of this file? [n]: "
    read answer

    if [ ${answer:=n} == "y" ]; then
        $realrm -rf *"$1"*
        echo "deleted"
    else 
        echo "additional copies retained"
    fi
else
    # Select the restore file
    restore="$(ls -d *"$1"*)"
    oname="$dest/$(echo $restore | cut -c16- ).restored"
    if [ -e "$oname" ]; then
        echo "\"$1\" already exists in this directory. Cannot overwrite." >&2
        exit 1
    fi

    echo -n "Restoring file \"$oname\" ..."
    $move "$restore" "$oname"
    echo done

fi

exit 0
