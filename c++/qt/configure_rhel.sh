#!/bin/bash

./configure \
    --prefix=/opt/local \
    -opensource \
    -release \
    -nomake examples \
    -nomake tests

