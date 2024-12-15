#!/bin/bash

set -e 

mkdir -p build

pushd build

cmake ..

popd

