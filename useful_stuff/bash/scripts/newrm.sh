#! /bin/sh

# newrm, a replacement for the existing rm command, provides a 
# rudimentary unremove capability by creating and utilizing a 
# directory within the user's home directory. It can handle 
# directories of content as well as individual files. If the
# user specifies -f, then it will delete and not archive.

# NOTE: Use cron to periodically delete the directory.

# Import our color ANSI codes
. ~/.scripts/initializeANSI.sh

mydir="$HOME/.deleted_files"
realrm="/bin/rm"
copy="/bin/cp -R"


# Make sure we have enough arguments
if [ $# -eq 0 ]; then # We have error condition, run command on real rm
    exec $realrm
fi

# Parse all command-line options, looking for -f
flags=""
while getopts "dfiPRrvW" opt
do
    case $opt in
    f ) # If you want to delete and not archive
        exec $realrm "$@"
    ;;
    v ) # Give information that this is my script
        echo -e "${cyanf}Archive rm script  ${redf}@author: Marvin Smith${reset}"
        echo
        echo

        # Print the real rm info
        echo -e "${cyanf}Real rm version information${reset}"
        exec $realrm --version
    ;;
    * ) 
        flags="$flags -$opt"
    ;;
    esac
done

# Remove the command-line arguments
shift $(( $OPTIND - 1 ))

# Make sure that our deleted file directory exists
if [ ! -d $mydir ]; then
    
    # Make sure we can write the the home directory
    if [ ! -w "$HOME" ]; then
        echo "$0 failed: can't create $mydir in $HOME" >&2
    fi

    # Create the directory
    mkdir $mydir

    # Give yourself sole rights
    chmod 700 $mydir 

fi

# Start moving data
for arg
do

    # Create the new name for the file
    newname="$mydir/$(date "+%S.%M.%H.%d.%m").$(basename "$arg")"
    
    # Copy the information
    if [ -f "$arg" ]; then
        $copy "$arg" $newname
    elif [ -d "$arg" ]; then
        $copy "$arg" $newname
    fi

done


# Delete the file
exec $realrm $flags "$@" 


