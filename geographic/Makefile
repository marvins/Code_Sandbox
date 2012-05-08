######################################################
#	 Makefile for GeoImage   -   MacOSX Validated    #
######################################################
#  RELEVANT LIBRARY FLAGS

# Final Install Destination
PREFIX = /opt/local

#Build directory   (DO NOT TOUCH -fpic)
BUILD = build

# Build flags 
CFLAGS = -g -Wall -fpic


#
# GDAL LIBRARY
#
GDAL_LIBS 		= -L/opt/local/lib -lgdal
GDAL_INCLUDE 	= -I/opt/local/include
GDAL         	= $(GDAL_LIBS) $(GDAL_INCLUDE)


#
# OpenCV Libraries
#
OPENCV = `pkg-config opencv --cflags --libs`
OPENCV_INCLUDE = `pkg-config opencv --cflags`


#
# Boost Libraries
#
BOOST_INCLUDE = -I/opt/local/include
BOOST_LIBS    = -L/opt/local/lib -lboost_filesystem -lboost_system
BOOST		  = $(BOOST_INCLUDE) $(BOOST_LIBS)


#
#  Unit Test Library
#
TEST_SOURCES = ../unit_test/src/Assert.cpp \
			   ../unit_test/src/Logger.cpp \
			   ../unit_test/src/Color.cpp \
			   ../useful_stuff/string_functions/string_utilities.cpp

TEST_HEADER  = ../unit_test/src \
			   ../useful_stuff/string_functions

TEST_INCLUDE = -I../unit_test/src -I../useful_stuff/string_functions
TEST = $(TEST_INCLUDE) $(TEST_SOURCES)

#
# Geographic NITF Libraries
#
HEADERS = src/structures/*.h \
	  	   src/utilities/*.h

OBJECTS = build/GeoImage.o \
		  build/GDAL2OpenCV.o \
		  build/OpenCVUtils.o \
		  build/GDAL_Data.o \
		  build/NITFHeader.o \
		  build/UInt16.o \
		  build/UInt8.o \
		  build/GeoHeader.o

GEO_INCLUDE = -Isrc/structures -Isrc/utilities


all: $(BUILD)/libgeoimage.so
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/bin
	mkdir -p $(BUILD)/lib
	mkdir -p $(BUILD)/include
	mkdir -p $(BUILD)/include/geoimage
	cp $(BUILD)/libgeoimage.so $(BUILD)/lib
	cp src/structures/*.h $(BUILD)/include/geoimage
	cp src/utilities/*.h  $(BUILD)/include/geoimage

###################################################################
#				BUILD SEQUENCE		
###################################################################
$(BUILD)/libgeoimage.so: $(OBJECTS)
	g++ -o $@ $(OBJECTS) $(GDAL) $(OPENCV) $(BOOST) -shared

build/GeoImage.o:	src/structures/GeoImage.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)

build/GDAL2OpenCV.o: src/utilities/GDAL2OpenCV.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)

build/OpenCVUtils.o: src/utilities/OpenCVUtils.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)

build/GDAL_Data.o:  src/structures/GDAL_Data.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)

build/NITFHeader.o: src/structures/NITFHeader.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)

build/GeoHeader.o: src/structures/GeoHeader.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(BOOST_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)

build/PixelType.o: src/structures/PixelType.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)

build/UInt16.o: src/structures/UInt16.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)

build/UInt8.o: src/structures/UInt8.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(CFLAGS) $(GEO_INCLUDE)


###################################################################
# 				MISC TOOLS
###################################################################
docs:
	doxygen 

clean: 
	rm -r build/*.o
	rm -r build/bin
	rm -r build/lib
	rm -r build/include
	rm unit_test

check:
	g++ -o build/unit_test unit_test.cpp $(OPENCV) $(TEST_SOURCES) -I$(PREFIX)/include/geoimage -L$(PREFIX)/lib -lgeoimage 
	./build/unit_test

install:
	cp -r $(BUILD)/bin $(PREFIX)
	cp -r $(BUILD)/lib $(PREFIX)
	cp -r $(BUILD)/include $(PREFIX)

uninstall:
	rm -r $(PREFIX)/lib/libgeoimage.so
	rm -r $(PREFIX)/include/geoimage

