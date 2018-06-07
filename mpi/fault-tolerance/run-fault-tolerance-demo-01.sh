#!/bin/sh

BUILD_PATH='release'

PRE_CMD=''

#  Parse CMD Args
for ARG in "$@"; do
    case $ARG in
        -d)
            echo "Using Debug Build"
            BUILD_PATH='debug'
            ;;

        --valgrind)
            echo "Using Valgrind"
            PRE_CMD='valgrind --leak-check=full'
            ;;
    esac
done

#  Starting the slave
CMD="./${BUILD_PATH}/fault-tolerance/mpi-fault-tolerance-demo-01-slave"


#  Start the master
CMD="mpirun -n 1 -hosts localhost ${PRE_CMD} ${BUILD_PATH}/fault-tolerance/mpi-fault-tolerance-demo-01-master"

echo "Starting Master: $CMD"
$CMD

