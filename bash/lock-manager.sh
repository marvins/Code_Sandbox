#!/bin/bash
#
#    File:    lock-manager.sh
#    Author:  Marvin Smith
#    Date:    4/24/2014
#
#    Purpose: Lock and unlock using the filesystem.
#
#    References: http://wiki.grzegorz.wierzowiecki.pl/code:mutex-in-bash
#

#-----------------------------#
#-           Usage           -#
#-----------------------------#
usage(){

    echo "usage: $0 [lock|unlock] [options]"
    echo ""
    echo "    lock   : Set the lock"
    echo "    unlock : Remove the lock"
    echo ""
    echo '    Options:'
    echo '    -h, --help              : Print usage instructions.'
    echo '    -l, --lockdir [dirname] : Directory for lockfile.'
    echo "                 DEFAULT=$LOCKDIR"
    echo '    -w, --wait              : Wait until lock is available.'
    echo '    -p, --pid [pid]         : Create optional PID file inside directory.'
    echo '    -m, --max [int]         : Set maximum number of synchronous calls.'
    echo "                 DEFAULT=${MAXPIDS}"
    echo ''

}


#------------------------------#
#-     Create the Lockfile    -#
#------------------------------#
run_lock(){
    
    #  pid value
    local maxpids=$MAXPIDS
    local usepid=$USE_PID
    local pidval=$PID_VALUE
    local LOCKDIR_LOCAL=$LOCKDIR
    
    
    # Start main loop
    while [ 1 ]; do
        
        #  set the lock directory
        if mkdir "${LOCKDIR_LOCAL}" &> /dev/null; then
        
            #  Create PID if necessary
            if [ "${usepid}" = '1' ]; then
                echo "Created pid file at ${LOCKDIR_LOCAL}/${pidval}.pid"
                echo "$pidval" > "${LOCKDIR_LOCAL}/${pidval}.`date "+%s"`.${RANDOM}.pid"
            else
                echo "$$" > "${LOCKDIR_LOCAL}/$$.`date "+%s"`.pid"
            fi

            return 0;
    
        #  If the lock failed, but we are below the count, then create the subdirectories
        elif [ ! "${usepid}" = '0' -a "`ls ${LOCKDIR_LOCAL} | wc -l`" -lt ${maxpids} ]; then
        
            #  Create the pid
            echo "$pidval" > "${LOCKDIR_LOCAL}/${pidval}.`date "+%s"`.${RANDOM}.pid"

            return 0;

        #  The lock failed, abort
        elif [ "$WAIT_FLAG" = '0' ]; then
            echo 'lock failed' 
            return 1

         fi

         # otherwise, the lock failed, but we are still waiting
         sleep 0.2
        
    done        

}

#-------------------------------#
#-     Remove the lockfile     -#
#-------------------------------#
run_unlock(){
   
    local pidval=$PID_VALUE

    #  Get rid of the lock directory
    if [ "`ls ${LOCKDIR} | wc -l`" -le '1' ]; then
        rm -rf "$LOCKDIR" &> /dev/null
    elif [ ! "${pidval}" = '0' ]; then
        echo "${pidval}"
        rm  ${LOCKDIR}/"`ls ${LOCKDIR} ${pidval}.* | tail -1`"
    else
        rm ${LOCKDIR}/"`ls ${LOCKDIR} | tail -1`"
    fi


    return $?
}

#----------------------------#
#-     Parse Command-Line   -#
#----------------------------#

#   Default Locking Directory
LOCKDIR='/var/tmp/temp-lock.lock'
LOCKDIR_FLAG=0

#   Wait Flag
WAIT_FLAG=0

#   Use PID
USE_PID=0
PID_FLAG=0
PID_VALUE=0
MAXPIDS=1
MAXPID_FLAG=0


#   Parse Command-Line
for ARG in "$@"; do
    case $ARG in

        #  Print usage instructions
        '-h' | '--help')
            usage
            exit 1
            ;;

        #  Wait Flag
        '-w' | '--wait' )
            WAIT_FLAG=1
            ;;

        #  Establish the lockfile
        'lock' )
            RUN_LOCK=1
            ;;

        #  Remove the lockfile
        'unlock' )
            RUN_UNLOCK=1
            ;;

        #  Set the desired lock directory
        '-l' | '--lockdir' )
            LOCKDIR_FLAG=1
            ;;

        #  Get the PID
        '-p' | '--pid' )
            PID_FLAG=1
            ;;
        
        # Get the max pid
        '-m' | '--max' )
            MAXPID_FLAG=1
            ;;

        #  Otherwise check values or print an error message
        *)
            # If we need to grab the lock name
            if [ "$LOCKDIR_FLAG" = '1' ]; then
                LOCKDIR_FLAG=0
                LOCKDIR=$ARG

            #  If we need to grab the pid
            elif [ "$PID_FLAG" = '1' ]; then
                PID_FLAG=0
                USE_PID=1
                PID_VALUE=$ARG

            #  Max PID Flag
            elif [ "$MAXPID_FLAG" = '1' ]; then
                MAXPID_FLAG=0
                MAXPIDS=$ARG

            # Otherwise, there is an error and we need to exit
            else
                echo "Unknown argument: $ARG" 1>&2
                exit 1
            fi
            ;;
    esac
done

#   If maxpids is larger than 1, we need to require the use of PIDS
if [ $MAXPIDS -gt 1 -a "$USE_PID" = '0' ]; then
    echo 'Setting the max number of PIDs above 1 requies the use of PIDs.' 1>&2
    usage
    exit 1
fi

#------------------------------------------------------#
#-      Make sure that we don't lock and unlock       -#
#------------------------------------------------------#
if [ "$RUN_LOCK" = '1' -a "$RUN_UNLOCK" = '1' ]; then
    echo 'You may only run one command.' 1>&2
    usage
    exit 1

#--------------------------------#
#-           Run Lock           -#
#--------------------------------#
elif [ "$RUN_LOCK" = '1' ]; then
    run_lock
    exit $?

#--------------------------------#
#-          Run Unlock          -#
#--------------------------------#
elif [ "$RUN_UNLOCK" = '1' ]; then
    run_unlock
    exit $?

#---------------------------------------#
#-       Otherwise throw an error      -#
#---------------------------------------#
else
    echo 'You must run either lock or unlock' 1>&2
    usage
    exit 1
fi

