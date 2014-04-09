#!/bin/sh
#
#    Author:  Marvin Smith
#    Date:    4/8/2014
#
#    Purpose:  Install command-line utilities to the desired location
#

PREFIX=/opt/local/bin

cp binary2decimal "$PREFIX/"
cp decimal2binary "$PREFIX/"
cp time-convert   "$PREFIX/"

