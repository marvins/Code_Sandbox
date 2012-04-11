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



# Geographic NITF Libraries
HEADERS = src/structures/*.h \
	  	   src/utilities/*.h \
    		../opencv/*.h 

OBJECTS = build/GeoImage.o \
		  build/GDAL2OpenCV.o \
		  build/OpenCVUtils.o

###################################################################
#				BUILD SEQUENCE		
###################################################################
unit_test: unit_test.cpp $(TEST_INCLUDE) $(OBJECTS)
	g++ unit_test.cpp -o unit_test $(OPENCV) $(GDAL) $(TEST_SOURCES) $(OBJECTS)

build/GeoImage.o:	src/structures/GeoImage.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE)

build/GDAL2OpenCV.o: src/utilities/GDAL2OpenCV.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE) $(GDAL_INCLUDE)

build/OpenCVUtils.o: ../opencv/OpenCVUtils.cpp $(HEADERS)
	g++ $< -c -o $@ $(OPENCV_INCLUDE)

#$(SOURCES) -o unit_test $(GDAL_INCLUDE) $(TEST_INCLUDE) `pkg-config opencv --cflags --libs` $(GDAL_LIBS) -g
#$(HEADERS) $(SOURCES) unit_test.cpp
