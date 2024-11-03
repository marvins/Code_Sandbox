#!/bin/sh


#  Install Path
PREFIX=/opt/local

./configure --prefix=${PREFIX} \
	        --with-grass=yes \
            --with-poppler=yes \
            --with-python \
            --with-java

