######################################################
#	 Makefile for GeoImage   -   MacOSX Validated    #
######################################################
#  RELEVANT LIBRARY FLAGS

# Final Install Destination
PREFIX = build

#Build directory   (DO NOT TOUCH -fPic)
BUILD = build

# Build flags 
CFLAGS = -Wall 
DFLAGS = $(CFLAGS) -O0 -g
OFLAGS = $(CFLAGS) -O3

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

OBJECTS       = build/NITFHeader.o     		build/GeoHeader.o 		build/PixelType.o		 build/OpenCVUtils.o	    build/GDAL_Data.o
OBJECTS_DEBUG = build/NITFHeader_DEBUG.o	build/GeoHeader_DEBUG.o build/PixelType_DEBUG.o  build/OpenCVUtils_DEBUG.o  build/GDAL_Data_DEBUG.o

#build/GeoImage.o        build/GDAL2OpenCV.o   	  
#build/GeoImage_DEBUG.o  build/GDAL2OpenCV_DEBUG.o 

GEO_INCLUDE = -Isrc/structures -Isrc/utilities


all: $(BUILD)/libgeoimage.so $(BUILD)/libgeoimage_DEBUG.so
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/bin
	mkdir -p $(BUILD)/lib
	mkdir -p $(BUILD)/include
	mkdir -p $(BUILD)/include/geoimage
	cp $(BUILD)/libgeoimage.so $(BUILD)/lib
	cp $(BUILD)/libgeoimage_DEBUG.so $(BUILD)/lib
	cp src/GeoImage.h	  $(BUILD)/include/GeoImage.h
	cp src/structures/*.h $(BUILD)/include/geoimage
	cp src/utilities/*.h  $(BUILD)/include/geoimage

###################################################################
#				BUILD SEQUENCE		
###################################################################
# Main Shared Libraries
$(BUILD)/libgeoimage.so: $(OBJECTS)
	g++ -o $@ $(OBJECTS)       $(GDAL) $(OPENCV) $(BOOST) $(OFLAGS) -shared

$(BUILD)/libgeoimage_DEBUG.so: $(OBJECTS_DEBUG)
	g++ -o $@ $(OBJECTS_DEBUG) $(GDAL) $(OPENCV) $(BOOST) $(DFLAGS) -shared

# GeoImage Core
build/GeoImage.o:	src/structures/GeoImage.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/GeoImage_DEBUG.o:	src/structures/GeoImage.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# GDAL 2 OpenCV 
build/GDAL2OpenCV.o: src/utilities/GDAL2OpenCV.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/GDAL2OpenCV_DEBUG.o: src/utilities/GDAL2OpenCV.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# OpenCV Utilities
build/OpenCVUtils.o: src/utilities/OpenCVUtils.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/OpenCVUtils_DEBUG.o: src/utilities/OpenCVUtils.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# GDAL_Data
build/GDAL_Data.o:  src/structures/GDAL_Data.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/GDAL_Data_DEBUG.o:  src/structures/GDAL_Data.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# NITF Header
build/NITFHeader.o: src/structures/NITFHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/NITFHeader_DEBUG.o: src/structures/NITFHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# GeoHeader
build/GeoHeader.o: src/structures/GeoHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(BOOST_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/GeoHeader_DEBUG.o: src/structures/GeoHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(BOOST_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# PixelType
build/PixelType.o: 		 src/structures/PixelType.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/PixelType_DEBUG.o: src/structures/PixelType.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(DFLAGS) $(GEO_INCLUDE) 



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
	g++ -o build/unit_test unit_test.cpp TEST_structures.cpp TEST_utilities.cpp $(OPENCV) $(TEST_SOURCES) -I$(PREFIX)/include -L$(PREFIX)/lib -lgeoimage
	./build/unit_test

install:
	cp -r $(BUILD)/bin $(PREFIX)
	cp -r $(BUILD)/lib $(PREFIX)
	cp -r $(BUILD)/include $(PREFIX)

uninstall:
	rm -r $(PREFIX)/lib/libgeoimage.so
	rm -r $(PREFIX)/lib/libgeoimage_DEBUG.so
	rm -r $(PREFIX)/include/GeoImage.h
	rm -r $(PREFIX)/include/geoimage

