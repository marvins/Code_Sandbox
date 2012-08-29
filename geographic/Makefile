# TODO  Need to standardize GDAL include path

######################################################
#	 Makefile for GeoImage   -   MacOSX Validated    #
######################################################
#  RELEVANT LIBRARY FLAGS

# Final Install Destination
PREFIX = /opt/local
GEOIMAGE_INCLUDE=-I$(PREFIX)/include
GEOIMAGE_LIBS=-L$(PREFIX)/lib -lgeoimage


#Build directory   (DO NOT TOUCH -fPIC)
BUILD = build
BIN   = bin

GDAL_DIR = /usr

# Build flags 
CFLAGS = -Wall 
DFLAGS = $(CFLAGS) -O0 -g
OFLAGS = $(CFLAGS) -O3

#
# GDAL LIBRARY
#
GDAL_LIBS 	= -L$(GDAL_DIR)/lib -lgdal
GDAL_INCLUDE 	= -I$(GDAL_DIR)/include/gdal
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
# OpenGL Libraries
#
UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
  OPENGL_INCLUDE = 
  OPENGL_LIBS    = -framework OpenGL -framework GLUT
  OPENGL         = $(OPENGL_INCLUDE) $(OPENGL_LIBS)
else
  OPENGL_INCLUDE = -I/usr/include
  OPENGL_LIBS    = -L/usr/lib -lglut -lGLU -lGL -lGLEW
  OPENGL         = $(OPENGL_INCLUDE) $(OPENGL_LIBS)
endif


#
#  Unit Test Library
#
TEST_SOURCES = ../unit_test/src/Logger.cpp \
			   ../unit_test/src/Color.cpp \
			   ../useful_stuff/string_functions/string_utilities.cpp

TEST_HEADER  = ../unit_test/src \
			   ../useful_stuff/string_functions

TEST_INCLUDE = -I../unit_test/src -I../useful_stuff/string_functions
TEST = $(TEST_INCLUDE) $(TEST_SOURCES)

#
# Geographic NITF Libraries
#
HEADERS = src/core/*.h src/dem/*.h src/extensions/*.h src/image/*.h src/utilities/*.h

OBJECTS       = build/string_utils.o       build/GS2_Header.o        build/CoordinateLatLon.o        build/CoordinateBase.o        build/TACID.o		  build/DTEDUtils.o    	  build/DEM.o 	     build/SRTMHeader.o       build/DTEDHeader.o	 	  build/NITFHeader.o       build/DEMHeader.o       build/ImageHeader.o	     build/GeoHeader.o 	     build/PixelType.o	      build/OpenCVUtils.o	    build/GDAL_Data.o	    build/GeoImage.o       build/GDAL2OpenCV.o	
OBJECTS_DEBUG = build/string_utils_DEBUG.o build/GS2_Header_DEBUG.o  build/CoordinateLatLon_DEBUG.o  build/CoordinateBase_DEBUG.o  build/TACID_DEBUG.o build/DTEDUtils_DEBUG.o build/DEM_DEBUG.o  build/SRTMHeader_DEBUG.o build/DTEDHeader_DEBUG.o build/NITFHeader_DEBUG.o build/DEMHeader_DEBUG.o build/ImageHeader_DEBUG.o build/GeoHeader_DEBUG.o build/PixelType_DEBUG.o  build/OpenCVUtils_DEBUG.o build/GDAL_Data_DEBUG.o build/GeoImage_DEBUG.o build/GDAL2OpenCV_DEBUG.o

GEO_INCLUDE = -Isrc/coordinates -Isrc/core -Isrc/dem -Isrc/image -Isrc/extensions -Isrc/utilities

UTEST_SOURCES = tests/unit_test.cpp \
	        	tests/dem/TEST_dem.cpp \
				tests/extensions/TEST_extensions.cpp \
				tests/image/TEST_structures.cpp \
				tests/image/HeaderData/TEST_headers.cpp \
				tests/image/GeoImage/TEST_geoimage.cpp \
				tests/utilities/TEST_utilities.cpp 


UNAME := $(shell uname)
ifeq ($(UNAME),Darwin)
	LIB_TYPE=dylib
else
	LIB_TYPE=so
endif

all: $(BUILD)/libgeoimage.$(LIB_TYPE) $(BUILD)/libgeoimage_DEBUG.$(LIB_TYPE)
	mkdir -p $(BUILD)
	mkdir -p $(BUILD)/bin
	mkdir -p $(BUILD)/lib
	mkdir -p $(BUILD)/include
	mkdir -p $(BUILD)/include/geoimage
	cp $(BUILD)/libgeoimage.$(LIB_TYPE) 		$(BUILD)/lib
	cp $(BUILD)/libgeoimage_DEBUG.$(LIB_TYPE) $(BUILD)/lib
	cp src/GeoImage.h	  	$(BUILD)/include/GeoImage.h
	cp src/coordinates/*.h  $(BUILD)/include/geoimage
	cp src/core/*.h		  	$(BUILD)/include/geoimage
	cp src/dem/*.h		 	$(BUILD)/include/geoimage
	cp src/extensions/*.h   $(BUILD)/include/geoimage
	cp src/extensions/*.hpp $(BUILD)/include/geoimage
	cp src/image/*.h 		$(BUILD)/include/geoimage
	cp src/utilities/*.h  	$(BUILD)/include/geoimage

tools: $(BIN)/geo_viewer $(BIN)/geo_meta_parser
	mkdir -p $(BIN)

###################################################################
#				BUILD SEQUENCE		
###################################################################
VIEWER_HDR=                       src/tools/geo_viewer/Quaternion.h src/tools/geo_viewer/visualization.h  src/tools/geo_viewer/camera.h  src/tools/geo_viewer/Cube.h    src/tools/geo_viewer/vector.h  src/tools/geo_viewer/initialization.h   src/tools/geo_viewer/keyboard.h  src/tools/geo_viewer/Options.h
VIEWER_OBJ=$(BUILD)/geo_viewer.o  $(BUILD)/Quaternion.o             $(BUILD)/visualization.o              $(BUILD)/camera.o              $(BUILD)/Cube.o                $(BUILD)/vector.o              $(BUILD)/initialization.o               $(BUILD)/keyboard.o              $(BUILD)/Options.o

# Viewer Program
$(BIN)/geo_viewer: $(VIEWER_OBJ) $(VIEWER_HDR)
	g++ -o $@ $(VIEWER_OBJ) $(OPENGL) $(BOOST) -lgdal -I/opt/local/include -L/opt/local/lib `pkg-config opencv --cflags --libs` -lgeoimage


# Geo Metadata Parser Program
$(BIN)/geo_meta_parser:  src/tools/geo_meta_parser.cpp  src/tools/geo_meta_parser/*.cpp src/tools/geo_meta_parser/*.h
	g++ -o $@ $<  src/tools/geo_meta_parser/*.cpp -I/opt/local/include -I/usr/include/gdal  -L/opt/local/lib -lgeoimage -lncurses `pkg-config opencv --cflags --libs` -g -lboost_system -lboost_filesystem

# Main Shared Libraries
$(BUILD)/libgeoimage.so: $(OBJECTS)
	g++ -o $@ $(OBJECTS)       $(GDAL) $(OPENCV) $(BOOST) $(OFLAGS) -shared

$(BUILD)/libgeoimage_DEBUG.so: $(OBJECTS_DEBUG)
	g++ -o $@ $(OBJECTS_DEBUG) $(GDAL) $(OPENCV) $(BOOST) $(DFLAGS) -shared


# Main Shared Libraries
$(BUILD)/libgeoimage.dylib: $(OBJECTS)
	g++ -dynamiclib -install_name $(PREFIX)/lib/libgeoimage.dylib -o $@ $(OBJECTS)       $(GDAL) $(OPENCV) $(BOOST) $(OFLAGS)

$(BUILD)/libgeoimage_DEBUG.dylib: $(OBJECTS_DEBUG)
	g++ -dynamiclib -install_name $(PREFIX)/lib/libgeoimage.dylib -o $@ $(OBJECTS_DEBUG) $(GDAL) $(OPENCV) $(BOOST) $(DFLAGS)



# GeoImage Core
build/GeoImage.o:	src/image/GeoImage.cpp $(HEADERS)
	g++ $< -fPIC -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/GeoImage_DEBUG.o:	src/image/GeoImage.cpp $(HEADERS)
	g++ $< -fPIC -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# GDAL 2 OpenCV 
build/GDAL2OpenCV.o: src/utilities/GDAL2OpenCV.cpp $(HEADERS)
	g++ $< -fPIC -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/GDAL2OpenCV_DEBUG.o: src/utilities/GDAL2OpenCV.cpp $(HEADERS)
	g++ $< -fPIC -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# OpenCV Utilities
build/OpenCVUtils.o: src/utilities/OpenCVUtils.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/OpenCVUtils_DEBUG.o: src/utilities/OpenCVUtils.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# GDAL_Data
build/GDAL_Data.o:  src/image/GDAL_Data.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE) $(BOOST_INCLUDE)

build/GDAL_Data_DEBUG.o:  src/image/GDAL_Data.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE) $(BOOST_INCLUDE)

# NITF Header
build/NITFHeader.o: src/image/NITFHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/NITFHeader_DEBUG.o: src/image/NITFHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# GeoHeader
build/GeoHeader.o: src/image/GeoHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/GeoHeader_DEBUG.o: src/image/GeoHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# ImageHeader
build/ImageHeader.o: src/image/ImageHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/ImageHeader_DEBUG.o: src/image/ImageHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# DEMHeader
build/DEMHeader.o: src/image/DEMHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/DEMHeader_DEBUG.o: src/image/DEMHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# DTEDHeader
build/DTEDHeader.o: src/image/DTEDHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/DTEDHeader_DEBUG.o: src/image/DTEDHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# SRTMHeader
build/SRTMHeader.o: src/image/SRTMHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/SRTMHeader_DEBUG.o: src/image/SRTMHeader.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(BOOST_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# PixelType
build/PixelType.o: 	 src/image/PixelType.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/PixelType_DEBUG.o: src/image/PixelType.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE) 

# Dem model data
build/DEM.o:       src/dem/DEM.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/DEM_DEBUG.o: src/dem/DEM.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# DTED Utilities
build/DTEDUtils.o:   src/utilities/DTEDUtils.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(OFLAGS) $(BOOST_INCLUDE)

build/DTEDUtils_DEBUG.o:   src/utilities/DTEDUtils.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(DFLAGS) $(BOOST_INCLUDE)

# GS2 NITF header
build/TACID.o:       src/extensions/TACID.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS) $(GEO_INCLUDE)

build/TACID_DEBUG.o: src/extensions/TACID.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS) $(GEO_INCLUDE)

# Coordinate Base
build/CoordinateBase.o:       src/coordinates/CoordinateBase.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(OFLAGS)

build/CoordinateBase_DEBUG.o: src/coordinates/CoordinateBase.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(DFLAGS)

# LatLon Coordinates
build/CoordinateLatLon.o:       src/coordinates/CoordinateLatLon.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(OFLAGS)

build/CoordinateLatLon_DEBUG.o: src/coordinates/CoordinateLatLon.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(DFLAGS) 

# GS2_Header
build/GS2_Header.o:        src/extensions/GS2_Header.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE)  $(OFLAGS) $(GEO_INCLUDE)

build/GS2_Header_DEBUG.o:  src/extensions/GS2_Header.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE)  $(DFLAGS) $(GEO_INCLUDE)

# String Utilities
build/string_utils.o:       src/utilities/string_utils.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(BOOST_INCLUDE) $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(OFLAGS)

build/string_utils_DEBUG.o: src/utilities/string_utils.cpp $(HEADERS)
	g++ -fPIC $< -c -o $@ $(BOOST_INCLUDE) $(OPENCV_INCLUDE) $(GDAL_INCLUDE) $(DFLAGS)

###################################################################
#				   		      Geo Viewer						  #
###################################################################
$(BUILD)/geo_viewer.o: src/tools/geo_viewer.cpp $(VIEWER_HDR)
	g++ $< -c -o $@  $(OPENGL_INCLUDE) $(GDAL_INCLUDE) $(GEOIMAGE_INCLUDE)

$(BUILD)/initialization.o: src/tools/geo_viewer/initialization.cpp $(VIEWER_HDR)
	g++ $< -c -o $@  $(OPENGL_INCLUDE)

$(BUILD)/keyboard.o: src/tools/geo_viewer/keyboard.cpp $(VIEWER_HDR)
	g++ $< -c -o $@  $(OPENGL_INCLUDE) $(GDAL_INCLUDE) $(GEOIMAGE_INCLUDE)

$(BUILD)/Options.o:  src/tools/geo_viewer/Options.cpp $(VIEWER_HDR)
	g++ $< -c -o $@  $(GDAL_INCLUDE) $(GEOIMAGE_INCLUDE)

$(BUILD)/vector.o:   src/tools/geo_viewer/vector.cpp  $(VIEWER_HDR)
	g++ $< -c -o $@ $(OPENGL_INCLUDE)

$(BUILD)/Cube.o:	 src/tools/geo_viewer/Cube.cpp    $(VIEWER_HDR)
	g++ $< -c -o $@ $(OPENGL_INCLUDE) $(GDAL_INCLUDE) $(GEOIMAGE_INCLUDE)

$(BUILD)/camera.o:	 src/tools/geo_viewer/camera.cpp    $(VIEWER_HDR)
	g++ $< -c -o $@ $(OPENGL_INCLUDE)

$(BUILD)/visualization.o:	 src/tools/geo_viewer/visualization.cpp    $(VIEWER_HDR)
	g++ $< -c -o $@ $(OPENGL_INCLUDE) $(GDAL_INCLUDE) $(GEOIMAGE_INCLUDE)

$(BUILD)/Quaternion.o:  src/tools/geo_viewer/Quaternion.cpp $(VIEWER_HDR)
	g++ $< -c -o $@ 

###################################################################
# 				MISC TOOLS
###################################################################
.PHONY: docs
docs:
	doxygen 

clean: 
	rm -r build/*.o
	rm -r build/bin
	rm -r build/lib
	rm -r build/include
	rm -r bin/geo_viewer
	rm -r build/unit_test

check:
	g++ -o build/unit_test $(UTEST_SOURCES) $(GDAL) $(OPENCV) $(TEST) -I$(PREFIX)/include -I$(PREFIX)/include/geoimage -L$(PREFIX)/lib -lgeoimage_DEBUG $(DFLAGS)
	./build/unit_test

install:
	cp -r $(BUILD)/bin $(PREFIX)
	cp -r $(BUILD)/lib $(PREFIX)
	cp -r $(BUILD)/include $(PREFIX)

uninstall:
	rm -r $(PREFIX)/lib/libgeoimage.$(LIB_TYPE)
	rm -r $(PREFIX)/lib/libgeoimage_DEBUG.$(LIB_TYPE)
	rm -r $(PREFIX)/include/GeoImage.h
	rm -r $(PREFIX)/include/geoimage

