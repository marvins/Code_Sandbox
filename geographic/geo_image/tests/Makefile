
GDAL_INCL=-I/usr/include/gdal
GDAL_LIBS=-L/usr/lib -lgdal
GDAL= ${GDAL_INCL} ${GDAL_LIBS}

BOOST_LIB=-L/usr/lib -lboost_system -lboost_filesystem

OPENCV=`pkg-config opencv --cflags --libs`

TEST_SOURCES=tests/unit_test.cpp tests/image/TEST_image.cpp

FRAMEWORK_SOURCES=../../useful_stuff/cpp/unit_test/src
FRAMEWORK_INCLUDE=-I../../useful_stuff/cpp/unit_test/src

GEOIMAGE_LIBS=-I/opt/local/include -L/opt/local/lib -lGeoImage

all: $(TEST_SOURCES)
	g++ $(TEST_SOURCES) -o unit_test ${GEOIMAGE_LIBS} ${BOOST_LIB}  ${FRAMEWORK_SOURCES}/*.cpp ${FRAMEWORK_INCLUDE}  ${GDAL}  ${OPENCV}
	./unit_test
	rm unit_test

