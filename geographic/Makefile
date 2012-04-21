# Unit test library data
#
TEST_SOURCES = ../unit_test/src/Assert.cpp \
			   ../unit_test/src/Logger.cpp \
			   ../unit_test/src/Color.cpp

TEST_INCLUDE = ../unit_test/src


# GDAL LIBRARY
GDAL_LIBS 		= -L/usr/local/lib -lgdal
GDAL_INCLUDE 	= -I/usr/local/include
GDAL         	= $(GDAL_LIBS) $(GDAL_INCLUDE)

# OpenCV Libraries
OPENCV = `pkg-config opencv --cflags --libs`
OPENCV_INCLUDE = `pkg-config opencv --cflags`

# Boost Libraries
BOOST_INCLUDE = -I/usr/local/include
BOOST_LIBS    = -L/usr/local/lib -lboost_filesystem -lboost_system
BOOST		  = $(BOOST_INCLUDE) $(BOOST_LIBS)


# Geographic NITF Libraries
HEADERS = src/structures/*.h \
	  	   src/utilities/*.h

OBJECTS = build/GeoImage.o \
		  build/GDAL2OpenCV.o \
		  build/OpenCVUtils.o \
		  build/GDAL_Data.o \
		  build/NITFHeader.o \
		  build/GeoHeader.o

###################################################################
#				BUILD SEQUENCE		
###################################################################
unit_test: unit_test.cpp $(TEST_INCLUDE) $(OBJECTS)
	g++ unit_test.cpp -o unit_test $(OPENCV) $(GDAL) $(BOOST) $(TEST_SOURCES) $(OBJECTS)

build/GeoImage.o:	src/structures/GeoImage.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE)

build/GDAL2OpenCV.o: src/utilities/GDAL2OpenCV.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE)

build/OpenCVUtils.o: src/utilities/OpenCVUtils.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE)

build/GDAL_Data.o:  src/structures/GDAL_Data.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE)

build/NITFHeader.o: src/structures/NITFHeader.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE)

build/GeoHeader.o: src/structures/GeoHeader.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(BOOST_INCLUDE)

###################################################################
# 				MISC TOOLS
###################################################################
clean: 
	rm -r build/*.o
	rm unit_test


