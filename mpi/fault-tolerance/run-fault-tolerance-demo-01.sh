#!/bin/sh

BUILD_PATH='release'


#  Parse CMD Args
for ARG in "$@"; do
    case $ARG in
        -d)
            BUILD_PATH='debug'
    esac
done

CMD="mpirun -n 3 -hosts localhost ${BUILD_PATH}/fault-tolerance/mpi-fault-tolerance-demo-01"

echo "Running: $CMD"
$CMD

